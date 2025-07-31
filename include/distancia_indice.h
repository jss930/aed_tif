#include <stdio.h>

struct DistanciaIndice {
    float distancia;
    int idx;
};

bool compararDistancia(const DistanciaIndice &a, const DistanciaIndice &b) {
    return a.distancia < b.distancia;
}