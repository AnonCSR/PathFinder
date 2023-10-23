#include "tensor_buffer_manager.h"

#include <cstdlib>

#include "macros/aligned_alloc.h"
#include "query/exceptions.h"
#include "storage/file_manager.h"

using namespace std;

TensorBufferManager::TensorBufferManager(FileId file_id, const uint_fast32_t num_pages, const uint_fast32_t page_size) :
    file_id(file_id),
    clock_pos(0),
    buffer_pool(new TensorPage[num_pages]),
    bytes(reinterpret_cast<char*>(PF_ALIGNED_ALLOC(4096, page_size * num_pages))),
    shared_buffer_pool_size(num_pages),
    page_size(page_size)
{
    pages.reserve(shared_buffer_pool_size);
}


TensorBufferManager::~TensorBufferManager() {
    flush();
    delete[](buffer_pool);
    PF_ALIGNED_FREE(bytes);
}

void TensorBufferManager::flush() {
    // flush() is always called at destruction.
    // this is important to check to avoid segfault when program terminates before calling init()
    assert(buffer_pool != nullptr);
    for (uint_fast32_t i = 0; i < shared_buffer_pool_size; i++) {
        auto& page = buffer_pool[i];
        assert(page.pins == 0);
        if (page.dirty) {
            file_manager.flush_big_page(file_id, page.get_page_number(), page.get_bytes(), page_size);
            page.dirty = false;
        }
    }
}


TensorPage& TensorBufferManager::get_last_page() {
    auto page_count = file_manager.count_pages(file_id);
    if (page_count == 0) {
        return get_page(0);
    } else {
        return get_page(page_count - 1);
    }
}


TensorPage& TensorBufferManager::append_page() {
    return get_page(file_manager.count_pages(file_id));
}


uint_fast32_t TensorBufferManager::get_buffer_available() {
    while (true) {
        // when pins == 0 the are no synchronization problems with pins and usage
        if (buffer_pool[clock_pos].pins == 0) {
            if (buffer_pool[clock_pos].usage == 0) {
                break;
            } else {
                buffer_pool[clock_pos].usage--;
            }
        }
        clock_pos++;
        clock_pos = clock_pos < shared_buffer_pool_size ? clock_pos : 0;
    }
    return clock_pos;
}

TensorPage& TensorBufferManager::get_page(uint_fast32_t page_number) noexcept {
    const PageId page_id(file_id, page_number);

    std::lock_guard<std::mutex> lck(shared_buffer_mutex);
    auto                        it = pages.find(page_id);

    if (it == pages.end()) {
        const auto buffer_available = get_buffer_available();
        auto&      page             = buffer_pool[buffer_available];
        if (page.page_id.file_id.id != FileId::UNASSIGNED) {
            pages.erase(page.page_id);
        }

        if (page.dirty) {
            file_manager.flush_big_page(file_id, page.get_page_number(), page.get_bytes(), page_size);
            page.dirty = false;
        }
        page.reassign(page_id, &bytes[buffer_available * page_size]);

        file_manager.read_big_page(page_id, page.get_bytes(), page_size);
        pages.insert({ page_id, &page });
        return page;
    } else { // page is the buffer
        it->second->pins++;
        return *it->second;
    }
    // lock is released
}