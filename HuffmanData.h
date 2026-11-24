#pragma once
#include <cstdint>
#include <string>
#include "Node.h"

struct HuffmanData {
    Node* root = nullptr;
    std::uint64_t frequency[256] = {};
    std::string codes[256];
    std::uint64_t originalSize = 0;
};

HuffmanData GenerateHuffmanData(const std::string& filename);
