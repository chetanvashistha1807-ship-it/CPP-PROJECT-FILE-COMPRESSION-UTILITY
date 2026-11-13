
#include <iostream>
#include <string>
#include "HuffmanData.h"
#include"HuffmanTree.h"

using namespace std;

int main() {

    HuffmanData data = GenerateHuffmanData("123.txt");

    cout << "Huffman Codes:\n\n";

    for (int i = 0; i < 256; i++) {

        if (!data.codes[i].empty()) {
            cout << "Byte: " << i
                 << " | Frequency: "
                 << (data.root != nullptr ? data.codes[i].size() : 0)
                 << " | Code: " << data.codes[i] << endl;
        }
    }

    return 0;
}
