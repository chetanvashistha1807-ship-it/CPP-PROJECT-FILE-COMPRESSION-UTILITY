#include <iostream>
#include <string>
#include "encoding.h"

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cout << "Huffman File Utility\n"
                      << "Usage:\n"
                      << "  compressor c <input-file> [output.huf]\n"
                      << "  compressor d <input.huf> [output-file]\n";
            return 0;
        }

        std::string mode = argv[1];
        if (mode == "c" && (argc == 3 || argc == 4)) {
            std::string output = argc == 4 ? argv[3] : std::string(argv[2]) + ".huf";
            encodeFile(argv[2], output);
        } else if (mode == "d" && (argc == 3 || argc == 4)) {
            std::string output = argc == 4 ? argv[3] : std::string(argv[2]) + ".out";
            decodeFile(argv[2], output);
        } else {
            std::cerr << "Invalid arguments. Run compressor without arguments for help.\n";
            return 1;
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
