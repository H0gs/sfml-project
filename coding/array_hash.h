//THIS IS ENTIRELY CHATGPT CODE

#pragma once

#include <array>
#include <functional>
#include "FakePlatform.h"  // Only if needed for std::hash<std::array<FakePlatform, 2>>

namespace std {
    template <>
    struct hash<std::array<FakePlatform, 2>> {
        std::size_t operator()(const std::array<FakePlatform, 2>& arr) const {
            std::size_t h1 = std::hash<FakePlatform>()(arr[0]);
            std::size_t h2 = std::hash<FakePlatform>()(arr[1]);
            return h1 ^ (h2 << 1);
        }
    };

    // Optional: if you also use unordered_set<array<int, 2>>
    template <>
    struct hash<std::array<int, 2>> {
        std::size_t operator()(const std::array<int, 2>& arr) const {
            return std::hash<int>()(arr[0]) ^ (std::hash<int>()(arr[1]) << 1);
        }
    };
}
