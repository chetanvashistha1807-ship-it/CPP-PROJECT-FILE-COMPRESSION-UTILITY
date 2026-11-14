#include"HuffmanTree.h"
#include"HuffmanData.h"
#include<vector>
#include<queue>
#include<string>
#include <algorithm>
#include<iostream>
#include<fstream>
using namespace std;

Node* BuildHuffmanTree(const int frequency[256]){
    priority_queue<Node*,vector<Node*>,Compare> pq; //use it to combine 2 lowest frequency characters
    for(int i = 0; i<256; i++){
        if(frequency[i]>0){
            Node* newNode = new Node(i,frequency[i]);
            pq.push(newNode);
        }
    }

    while(pq.size()>1){
         Node* a = pq.top();
         pq.pop();
         Node* b = pq.top();
         pq.pop();

         Node*c = new Node('\0', a->frequency+b->frequency);
         c->left = a;
         c->right = b;
         
         pq.push(c);
    }
    
    if(pq.empty()){
        return nullptr;   //condition for edge case when the priority queue is empty.
    }
    Node* x = pq.top();
    return x;

}

void GenerateCodes(Node* root, string code, string codes[256]){
    if(root==nullptr)return;
    if(root->left==nullptr && root->right==nullptr){
        if(code.empty()) code = "0"; //we will assign 0 as a code to some file which only have one character multiple times;
        codes[root->character] = code;

        return;

    }
    GenerateCodes(root->left, code+ "0" , codes);
    GenerateCodes(root->right, code + "1", codes);

}

HuffmanData GenerateHuffmanData(const string& filename){
   
   ifstream file(filename, ios::binary); //ios::binary opens the file in binary reading mode
   if(!file.is_open()){
    throw runtime_error("file is not opening");
   }

   int frequency[256] = {0};
   char ch;
   while(file.get(ch)){//read character by character and store it into "ch".
        unsigned char byte = static_cast<unsigned char>(ch); //convert the signed char representation to unsigned char representation named byte
        frequency[byte]++;

    }

    Node* root = BuildHuffmanTree(frequency);
    string codes[256];
    GenerateCodes(root,"",codes);

    HuffmanData data;
    std::copy(codes, codes + 256, data.codes); //this copy all 256 strings into data.codes. normally assigning is not possible in cpp.
    data.root = root;

    file.close();

    
   return data;
       
}

