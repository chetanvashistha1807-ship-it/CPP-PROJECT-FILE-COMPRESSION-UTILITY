#pragma once
#include "Node.h"

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;   //this will work as a comparator in our priority queue.
    }
};