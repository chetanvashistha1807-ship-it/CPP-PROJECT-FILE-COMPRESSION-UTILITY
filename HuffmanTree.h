#pragma once
#include "Node.h"
#include "Compare.h"
#include <cstdint>
#include <string>

Node* BuildHuffmanTree(const std::uint64_t frequency[256]);
void GenerateCodes(Node* root, const std::string& code, std::string codes[256]);
void DeleteTree(Node* root);
