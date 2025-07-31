#ifndef __NODE_HASH__
#define __NODE_HASH__

#include "../node_element.h"

struct NodeHash {
    int operator()(const NodeElement &node) const {
        return node.id;  // Asume que el ID ya es único y adecuado como clave
    }
};

#endif
