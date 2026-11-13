#pragma once
#include"Node.h"
#include"Compare.h"
#include<string>
using namespace std;

Node* BuildHuffmanTree(const int frequency[256]);
void GenerateCodes(Node* root, string code, string codes[256]);