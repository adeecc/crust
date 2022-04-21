#pragma once
#include <cstdint>

class UID {
    UID() = default;

   public:
    UID(const UID&) = delete;
    UID& operator=(const UID&) = delete;
    ~UID() = delete;

    static std::uint64_t generate() {
        static std::uint64_t current_id = 0;

        return current_id++;
    }
};
