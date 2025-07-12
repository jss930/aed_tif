template <class T>
class Edge {
public:
    typedef typename T::E E;
    typedef typename T::node node;
    node *nodes[2];
    E data;
};