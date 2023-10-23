#include "cursor.h"

#include <cassert>
#include <iostream>

#include "api/result_parser.h"

using namespace API;

void Cursor::execute(const std::string& query) {
    boost::asio::ip::tcp::socket socket(connection.io_service);
    boost::asio::ip::tcp::resolver resolver(connection.io_service);

    // TODO: try catch for possible errors
    boost::asio::connect(socket, resolver.resolve({connection.host, connection.port}));

    auto query_length = query.size();

    unsigned char size_buffer[CommunicationProtocol::BYTES_FOR_QUERY_LENGTH];
    for (int i = 0, offset = 0; i < CommunicationProtocol::BYTES_FOR_QUERY_LENGTH; i++, offset += 8) {
        unsigned char c = (query_length >> offset) & 0xFF;
        size_buffer[i] = c;
    }
    assert(CommunicationProtocol::BYTES_FOR_QUERY_LENGTH == 4);
    unsigned char cursor_mask = 0b1000'0000;
    // mark as cursor querys
    size_buffer[3] = size_buffer[3] | cursor_mask;

    // send query length
    boost::asio::write(
        socket,
        boost::asio::buffer(size_buffer, CommunicationProtocol::BYTES_FOR_QUERY_LENGTH)
    );
    // send query
    boost::asio::write(
        socket,
        boost::asio::buffer(query.data(), query_length)
    );

    // receive binding size
    boost::asio::read(
        socket,
        boost::asio::buffer(size_buffer, CommunicationProtocol::BUFFER_SIZE)
    );

    auto binding_size = static_cast<unsigned int>(size_buffer[0])
                      + (static_cast<unsigned int>(size_buffer[1]) << 8)
                      + (static_cast<unsigned int>(size_buffer[2]) << 16);
    conn = std::make_unique<CursorConnection>(std::move(socket), binding_size);
}


std::unique_ptr<std::vector<std::vector<GraphObject>>> Cursor::get_results() {
    assert(conn != nullptr);

    std::vector<std::vector<GraphObject>> results;
    ResultParser result_parser(results, conn->binding_size);
    // TODO: how to manage errors/exceptions?
    // maybe assume flush before error and then a clean error
    do {
        boost::asio::read(
            conn->socket,
            boost::asio::buffer(buffer, CommunicationProtocol::BUFFER_SIZE)
        );
        // We print skipping first 3 bytes, which are used to indicate the status and the length of the message
        auto reply_length = static_cast<unsigned int>(buffer[1])
                            + (static_cast<unsigned int>(buffer[2]) << 8);

        // process (buffer+3) to (buffer+reply_length) byte per byte
        auto ptr_end = buffer+reply_length;
        for (auto ptr = buffer+3; ptr < ptr_end; ptr++) {
            result_parser.process_byte(*ptr);
        }
    } while ( !CommunicationProtocol::last_message(buffer[0]) );
    result_parser.end();

    return std::make_unique<std::vector<std::vector<GraphObject>>>(
        std::move(results)
    );

}


std::unique_ptr<std::vector<GraphObject>> Cursor::fetchone() {
    if (conn == nullptr) return nullptr;

    // First 4 Bytes is the N
    buffer[0] = 1;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;
    boost::asio::write(
        conn->socket,
        boost::asio::buffer(buffer, 4)
    );
    auto results = get_results();

    if (results != nullptr && results->size() > 0) {
        std::vector<GraphObject> first_binding = std::move(results->at(0));
        return std::make_unique<std::vector<GraphObject>>(std::move(first_binding));
    } else {
        return nullptr;
    }
}

std::unique_ptr<std::vector<std::vector<GraphObject>>> Cursor::fetchmany(uint_fast32_t N) {
    if (conn == nullptr) return nullptr;

    // encode N in buffer
    for (int i = 0, offset = 0; i < 4; i++, offset += 8) {
        unsigned char c = (N >> offset) & 0xFF;
        buffer[i] = c;
    }

    boost::asio::write(
        conn->socket,
        boost::asio::buffer(buffer, 4)
    );
    return get_results();
}