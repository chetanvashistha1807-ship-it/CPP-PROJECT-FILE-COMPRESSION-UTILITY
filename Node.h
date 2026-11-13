#pragma once

struct Node{
    unsigned char character;
    int frequency;

    Node* left;
    Node* right;

    Node(unsigned char ch, int freq);
};