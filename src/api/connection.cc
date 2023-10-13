#include "connection.h"

#include "cursor.h"

using namespace API;

std::unique_ptr<Cursor> Connection::cursor() {
    return std::make_unique<Cursor>(*this);
}

std::unique_ptr<Cursor> Connection::execute(const std::string& query) {
    auto cursor = std::make_unique<Cursor>(*this);
    cursor->execute(query);
    return cursor;
}
