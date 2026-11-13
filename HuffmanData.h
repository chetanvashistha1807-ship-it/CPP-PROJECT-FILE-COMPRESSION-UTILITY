#pragma once
#include<string>
#include"HuffmanTree.h"
using namespace std;

struct HuffmanData{
    Node* root;
    int frequency[256];
    string codes[256];

};

HuffmanData GenerateHuffmanData(const string& filename); //definition of this function is in Huffmantree.cpp file
