#ifndef __NODE_ELEMENT__
#define __NODE_ELEMENT__

#include <vector>
#include <SDL3/SDL.h>

struct NodeElement {
    int id;
    SDL_FPoint position;
    std::vector<int> vecinos;

    NodeElement() : id(-1), position{0, 0} {}
    NodeElement(int _id) : id(_id), position{0, 0} {}
    NodeElement(int _id, float x, float y) : id(_id), position{x, y} {}

    bool operator==(const NodeElement &other) const {
        return id == other.id;
    }
};

#endif