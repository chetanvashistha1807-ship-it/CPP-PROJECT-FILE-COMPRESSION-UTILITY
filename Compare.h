#pragma once
#include "Node.h"

struct Compare {
    bool operator()(const Node* a, const Node* b) const {
        return a->frequency > b->frequency;
    }
};
