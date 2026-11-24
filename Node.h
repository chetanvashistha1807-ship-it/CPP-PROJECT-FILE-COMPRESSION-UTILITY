#pragma once
#include <cstdint>

struct Node {
    unsigned char character;
    std::uint64_t frequency;
    Node* left;
    Node* right;

    Node(unsigned char ch, std::uint64_t freq);
};
