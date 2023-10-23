#pragma once

#include <atomic>
#include <cassert>

#include "storage/page_id.h"

/* TensorPage is an adaptation of the regular Page class from src/storage/page.h, specifically for handling tensor files
 * and interact with the TensorBufferManager
 */
class TensorPage {
    friend class TensorBufferManager;

public:
    // Contains file_id and page_number
    PageId page_id;

    // Mark as dirty so when page is replaced it is written back to disk
    inline void make_dirty() noexcept {
        dirty = true;
    }

    // Get the start memory position of the allocated bytes
    inline char* get_bytes() const noexcept {
        return bytes;
    }

    inline uint32_t get_page_number() const noexcept {
        return page_id.page_number;
    };

private:
    // Start memory address of the page
    char* bytes;

    // Count of objects using this page, modified only by buffer_manager
    std::atomic<uint32_t> pins;

    // Used by the replacement policy
    std::atomic<uint32_t> usage;

    // True if data in memory is different from disk
    bool dirty;

    TensorPage() noexcept : page_id(FileId(FileId::UNASSIGNED), 0), bytes(nullptr), pins(0), usage(0), dirty(false) { }

    void pin() noexcept {
        pins++;
        usage++;
    }

    void unpin() noexcept {
        assert(pins > 0 && "Cannot unpin if pin count is 0");
        pins--;
    }

    // Only meant for buffer_manager.remove()
    void reset() noexcept {
        assert(pins == 0 && "Cannot reset page if it is pinned");
        this->bytes   = nullptr;
        this->page_id = PageId(FileId(FileId::UNASSIGNED), 0);
        this->pins    = 0;
        this->usage   = 0;
        this->dirty   = false;
    }

    void reassign(PageId page_id, char* bytes) noexcept {
        assert(!dirty && "Cannot reassign page if it is dirty");
        assert(pins == 0 && "Cannot reassign page if it is pinned");
        assert(usage == 0 && "Should not reassign page if usage is not 0");

        this->page_id = page_id;
        this->bytes   = bytes;
        this->pins    = 1;
        this->usage   = 1;
    }
};
