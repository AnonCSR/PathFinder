#pragma once

#include <cassert>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "storage/file_id.h"
#include "tensor_page.h"
#include "third_party/robin_hood/robin_hood.h"

/*
 * TensorBufferManager contains all the tensor pages in memory and is used to get a page, making transparent if the page
 * is already in memory or needs to be read from disk.
 *
 * Unlike the global buffer_manager, this class manages a single TensorStore per instance.
 */
class TensorBufferManager {
public:
    static constexpr uint64_t DEFAULT_BUFFER_SIZE = 1024 * 1024 * 1024; // 1GB

    TensorBufferManager(FileId file_id, uint_fast32_t num_pages, uint_fast32_t page_size);

    ~TensorBufferManager();

    // Get a page. It will search in the shared buffer and if it is not on it, it will read from disk and put in the
    // buffer. Also it will pin the page, so calling buffer_manager.unpin(page) is expected when the caller doesn't need
    // the returned page anymore.
    TensorPage& get_page(uint_fast32_t page_number) noexcept;

    // Similar to get_page, but the page_number is the greatest number such that page number exist on disk.
    TensorPage& get_last_page();

    // Similar to get_page, but the page_number is the smallest number such that page number does not exist on disk.
    // The page returned has all its bytes initialized to 0. This operation perform a disk write immediately
    // so 2 append_page in a row will work as expected.
    TensorPage& append_page();

    // Write all dirty pages to disk
    void flush();

    // Increases the count of objects using the page. When you get a page using the methods get_page or get_tmp_page
    // the page is already pinned, so you shouldn't call this method unless you want to pin the page more than once
    void pin(TensorPage& page) {
        page.pin();
    }

    // reduces the count of objects using the page. Should be called when a object using the page is destroyed.
    void unpin(TensorPage& page) {
        page.unpin();
    }

    constexpr auto get_shared_buffer_pool_size() const noexcept {
        return shared_buffer_pool_size;
    }

    uint_fast32_t get_page_size() const noexcept {
        return page_size;
    }

private:
    FileId file_id;
    // Simple clock used to page replacement in the shared buffer
    uint_fast32_t clock_pos;

    // Array of `buffer_pool_size` pages
    TensorPage* const buffer_pool;

    // Beginning of the allocated memory for the pages of the shared buffer
    char* const bytes;

    std::mutex shared_buffer_mutex;

    // Maximum pages the buffer can have
    const uint_fast32_t shared_buffer_pool_size;

    // Size of each page
    const uint_fast32_t page_size;

    // Used to search the index in the `buffer_pool` of a certain page
    robin_hood::unordered_map<PageId, TensorPage*> pages;

    // Returns the index of an unpinned page from shared buffer (`buffer_pool`)
    uint_fast32_t get_buffer_available();
};