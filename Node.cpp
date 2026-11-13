#include"Node.h"

Node::Node(unsigned char ch, int freq){
    character = ch;
    frequency = freq;

    left = nullptr;
    right = nullptr;
}