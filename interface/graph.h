template <class _T>
class Graph {
public:
    typedef typename _T::E E;
    typedef typename _T::N N;
    typedef typename _T::node node;
    typedef typename _T::edge edge;

    list<node*> vertices; // Cambiar por Tablas Hash, AVl o Red-Black Trees
    // list<edge*> edges;

    void addNode(N data) {
        vertices.push_back(new node(data));
    }

    void addEdge(E data, node* n1, node* n2) {
        edge* e = new edge();
        e->data = data;
        e->nodes[0] = n1;
        e->nodes[1] = n2;
        edges.push_back(e);
        n1->adjacency.push_back(e);
        n2->adjacency.push_back(e);
    }
};