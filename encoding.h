#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "HuffmanData.h"
#include "HuffmanTree.h"

namespace huffman_detail {
constexpr char MAGIC[4] = {'H', 'U', 'F', '1'};
constexpr unsigned char HUFFMAN_MODE = 0;
constexpr unsigned char RAW_MODE = 1;

inline void writeU64(std::ostream& out, std::uint64_t value) {
    for (int i = 0; i < 8; ++i)
        out.put(static_cast<char>((value >> (i * 8)) & 0xFF));
    if (!out) throw std::runtime_error("Failed writing compressed file.");
}
inline std::uint64_t readU64(std::istream& in) {
    std::uint64_t value = 0;
    for (int i = 0; i < 8; ++i) {
        int byte = in.get();
        if (byte == EOF) throw std::runtime_error("Compressed file is truncated.");
        value |= static_cast<std::uint64_t>(static_cast<unsigned char>(byte)) << (i * 8);
    }
    return value;
}
inline void writeHeader(std::ostream& out, unsigned char mode, std::uint64_t size) {
    out.write(MAGIC, 4);
    out.put(static_cast<char>(mode));
    writeU64(out, size);
    if (!out) throw std::runtime_error("Failed writing compressed file header.");
}
inline void writePackedBits(std::istream& input, std::ostream& output,
                            const std::string codes[256]) {
    unsigned char byte = 0;
    int bits = 0;
    char ch;
    while (input.get(ch)) {
        const std::string& code = codes[static_cast<unsigned char>(ch)];
        for (char bit : code) {
            byte = static_cast<unsigned char>((byte << 1) | (bit == '1'));
            if (++bits == 8) {
                output.put(static_cast<char>(byte));
                byte = 0;
                bits = 0;
            }
        }
    }
    if (!input.eof()) throw std::runtime_error("Error reading input during encoding.");
    if (bits != 0) {
        byte = static_cast<unsigned char>(byte << (8 - bits));
        output.put(static_cast<char>(byte));
    }
    if (!output) throw std::runtime_error("Failed writing encoded data.");
}
inline void printStats(std::uint64_t original, std::uint64_t compressed) {
    std::cout << "Original size:   " << original << " bytes\n"
              << "Compressed size: " << compressed << " bytes\n";
    if (original == 0) {
        std::cout << "Space saved:     0% (empty file)\n";
    } else {
        double saved = (1.0 - static_cast<double>(compressed) / original) * 100.0;
        std::cout << "Space change:    " << saved << "%\n";
    }
}
} // namespace huffman_detail

inline void encodeFile(const std::string& filename,
                       const std::string& outputFilename = "compressed.huf") {
    using namespace huffman_detail;
    HuffmanData data = GenerateHuffmanData(filename);

    std::uint64_t bitCount = 0;
    for (int i = 0; i < 256; ++i) {
        if (data.frequency[i] && data.codes[i].size() >
            (std::numeric_limits<std::uint64_t>::max() - bitCount) / data.frequency[i]) {
            DeleteTree(data.root);
            throw std::runtime_error("Encoded size overflow.");
        }
        bitCount += data.frequency[i] * data.codes[i].size();
    }
    const std::uint64_t huffmanSize = 13 + 256 * 8 + (bitCount + 7) / 8;
    const std::uint64_t rawSize = 13 + data.originalSize;
    const bool useRaw = rawSize >= 13 && (huffmanSize >= rawSize);

    std::ifstream input(filename, std::ios::binary);
    if (!input) {
        DeleteTree(data.root);
        throw std::runtime_error("Could not reopen input file.");
    }
    std::ofstream output(outputFilename, std::ios::binary);
    if (!output) {
        DeleteTree(data.root);
        throw std::runtime_error("Could not create output file: " + outputFilename);
    }

    writeHeader(output, useRaw ? RAW_MODE : HUFFMAN_MODE, data.originalSize);
    if (useRaw) {
        output << input.rdbuf();
    } else {
        for (int i = 0; i < 256; ++i) writeU64(output, data.frequency[i]);
        writePackedBits(input, output, data.codes);
    }
    if (!output) {
        DeleteTree(data.root);
        throw std::runtime_error("Failed writing output file.");
    }
    output.close();
    DeleteTree(data.root);

    std::ifstream check(outputFilename, std::ios::binary | std::ios::ate);
    if (!check) throw std::runtime_error("Could not inspect compressed output.");
    std::uint64_t compressedSize = static_cast<std::uint64_t>(check.tellg());
    std::cout << "Compression completed: " << outputFilename << "\n"
              << "Storage mode: " << (useRaw ? "raw (compression would be larger)" : "Huffman") << "\n";
    printStats(data.originalSize, compressedSize);
}

inline void decodeFile(const std::string& compressedFilename,
                       const std::string& outputFilename = "decompressed.out") {
    using namespace huffman_detail;
    std::ifstream input(compressedFilename, std::ios::binary);
    if (!input) throw std::runtime_error("Could not open compressed file: " + compressedFilename);

    char magic[4];
    input.read(magic, 4);
    if (input.gcount() != 4 || !std::equal(magic, magic + 4, MAGIC))
        throw std::runtime_error("Invalid compressed file (bad signature).");
    int modeValue = input.get();
    if (modeValue == EOF) throw std::runtime_error("Compressed file is truncated.");
    unsigned char mode = static_cast<unsigned char>(modeValue);
    std::uint64_t originalSize = readU64(input);

    std::ofstream output(outputFilename, std::ios::binary);
    if (!output) throw std::runtime_error("Could not create output file: " + outputFilename);

    if (mode == RAW_MODE) {
        std::uint64_t copied = 0;
        char buffer[8192];
        while (copied < originalSize) {
            std::uint64_t remaining = originalSize - copied;
            std::streamsize amount = static_cast<std::streamsize>(
                std::min<std::uint64_t>(sizeof(buffer), remaining));
            input.read(buffer, amount);
            std::streamsize got = input.gcount();
            if (got != amount) throw std::runtime_error("Raw data section is truncated.");
            output.write(buffer, got);
            copied += static_cast<std::uint64_t>(got);
        }
    } else if (mode == HUFFMAN_MODE) {
        std::uint64_t frequency[256] = {};
        std::uint64_t total = 0;
        for (int i = 0; i < 256; ++i) {
            frequency[i] = readU64(input);
            if (frequency[i] > std::numeric_limits<std::uint64_t>::max() - total)
                throw std::runtime_error("Invalid frequency table.");
            total += frequency[i];
        }
        if (total != originalSize)
            throw std::runtime_error("Invalid frequency table: size mismatch.");

        Node* root = BuildHuffmanTree(frequency);
        if (originalSize == 0) {
            if (root) { DeleteTree(root); throw std::runtime_error("Invalid empty-file tree."); }
        } else if (!root) {
            throw std::runtime_error("Missing Huffman tree.");
        }

        if (root && !root->left && !root->right) {
            for (std::uint64_t i = 0; i < originalSize; ++i)
                output.put(static_cast<char>(root->character));
        } else if (root) {
            Node* current = root;
            std::uint64_t produced = 0;
            int byteValue;
            while (produced < originalSize && (byteValue = input.get()) != EOF) {
                unsigned char byte = static_cast<unsigned char>(byteValue);
                for (int bit = 7; bit >= 0 && produced < originalSize; --bit) {
                    current = ((byte >> bit) & 1) ? current->right : current->left;
                    if (!current) {
                        DeleteTree(root);
                        throw std::runtime_error("Invalid Huffman bitstream.");
                    }
                    if (!current->left && !current->right) {
                        output.put(static_cast<char>(current->character));
                        ++produced;
                        current = root;
                    }
                }
            }
            if (produced != originalSize) {
                DeleteTree(root);
                throw std::runtime_error("Encoded data ended before the original file was reconstructed.");
            }
        }
        if (root) DeleteTree(root);
    } else {
        throw std::runtime_error("Unknown compression mode.");
    }

    output.close();
    if (!output) throw std::runtime_error("Failed writing decompressed output.");
    std::cout << "Decompression completed: " << outputFilename << "\n"
              << "Restored size: " << originalSize << " bytes\n";
}
