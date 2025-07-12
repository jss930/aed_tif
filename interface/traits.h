template <class _E, class _N>
class Traits {
public:
    typedef _E E;
    typedef _N N;
    typedef Traits<E,N> Self;
    typedef Node<self> node;
    typedef Edge<self> edge;
};