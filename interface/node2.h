template <class T>
class Node {
public:
    typedef typename T::N N;
    typedef typename T::edge edge;
    N data;
    list<edge*> adjacency;

    Node(N);
};