#include "HuffmanTree.h"
#include "HuffmanData.h"
#include <algorithm>
#include <fstream>
#include <queue>
#include <stdexcept>
#include <vector>

Node* BuildHuffmanTree(const std::uint64_t frequency[256]) {
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

    for (int i = 0; i < 256; ++i) {
        if (frequency[i] != 0)
            pq.push(new Node(static_cast<unsigned char>(i), frequency[i]));
    }

    if (pq.empty()) return nullptr;

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* parent = new Node(0, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    return pq.top();
}

void GenerateCodes(Node* root, const std::string& code, std::string codes[256]) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[root->character] = code.empty() ? "0" : code;
        return;
    }
    GenerateCodes(root->left, code + "0", codes);
    GenerateCodes(root->right, code + "1", codes);
}

void DeleteTree(Node* root) {
    if (!root) return;
    DeleteTree(root->left);
    DeleteTree(root->right);
    delete root;
}

HuffmanData GenerateHuffmanData(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Could not open input file: " + filename);

    HuffmanData data;
    char ch;
    while (file.get(ch)) {
        unsigned char byte = static_cast<unsigned char>(ch);
        ++data.frequency[byte];
        ++data.originalSize;
    }
    if (!file.eof()) throw std::runtime_error("Error while reading input file.");

    data.root = BuildHuffmanTree(data.frequency);
    GenerateCodes(data.root, "", data.codes);
    return data;
}
