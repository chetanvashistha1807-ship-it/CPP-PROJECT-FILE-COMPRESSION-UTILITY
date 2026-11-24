#include "Node.h"

Node::Node(unsigned char ch, std::uint64_t freq)
    : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
