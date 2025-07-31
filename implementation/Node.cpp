#include "general.h"
#include "Node.h"

Node::Node(float x, float y)
{
    shape.x = x;
    shape.y = y;
}
Node::Node()
{
}
Node::~Node()
{
}
/* bool Node::isInCircule(float x, float y)
{
    return 1;
    return (x >= shape.x and x <= shape.x + RADIUS &&
            y >= shape.y and y <= shape.y + RADIUS);
}
 */