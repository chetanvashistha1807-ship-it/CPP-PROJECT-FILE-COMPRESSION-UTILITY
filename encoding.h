#pragma once
#include<fstream>
#include<string>
#include <stdexcept>
#include"HuffmanData.h"
#include"HuffmanTree.h"
using namespace std;

void writeBytes(const string& codes, ofstream &outputFile){
   
    unsigned char byte = 0;

    int iteration = 0;
    
    for(int i = 0; i<codes.size(); i++){
        int bit = codes[i] - '0'; //codes[i] is in char datatype.. We will have to convert it into int before going. converting to int

        byte = (byte<<1) | bit;
        iteration++;
        if(iteration==8){
            outputFile.put(byte);     //8 bits check to write one byte into file.
            iteration = 0;
            byte = 0;
            continue;
        }
        
    }

    if(iteration>0 && iteration<8){         //logic for the last remaining bits. 
      byte = byte << (8-iteration);
      outputFile.put(byte);
    }

}

void encodeFile(const string& filename){
   ofstream outputFile("compressed.bin", ios::binary);
   HuffmanData data = GenerateHuffmanData(filename);
   ifstream file(filename, ios::binary);
   string encoded;
   char ch;
   while(file.get(ch)){
    unsigned char byte = ch;
    encoded += data.codes[byte]; //append not overwrite
    
   }

   writeBytes(encoded, outputFile);
   outputFile.close();

}

