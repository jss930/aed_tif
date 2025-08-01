#ifndef __GRID_H__
#define __GRID_H__

#include "Node.h"
#include "myvector.h"

#define CELL_SIZE 80 // tamaño de celda en píxeles

class Grid2D
{
private:
    int cols, rows;
    Vector<Node *> **cells; // matriz dinámica de punteros a vectores

public:
    Grid2D(float width, float height)
    {
        cols = (int)(width / CELL_SIZE) + 1;
        rows = (int)(height / CELL_SIZE) + 1;

        cells = new Vector<Node *> *[cols];
        for (int i = 0; i < cols; ++i)
        {
            cells[i] = new Vector<Node *>[rows];
        }
        printf("Grid: cols = %d, rows = %d\n", cols, rows);
    }

    ~Grid2D()
    {
        for (int i = 0; i < cols; ++i)
        {
            delete[] cells[i];
        }
        delete[] cells;
    }

    void insert(Node *node)
    {
        int cx = (int)(node->shape.x / CELL_SIZE);
        int cy = (int)(node->shape.y / CELL_SIZE);

        if (cx < 0 || cx >= cols || cy < 0 || cy >= rows)
        {
            printf("[!] Nodo fuera del grid: cx=%d, cy=%d\n", cx, cy);
            return;
        }

        if (cx >= 0 && cx < cols && cy >= 0 && cy < rows)
        {
            cells[cx][cy].push_back(node);
        }
    }

    void getNearby(Node *node, Vector<Node *> &neighbors)
    {
        int cx = (int)(node->shape.x / CELL_SIZE);
        int cy = (int)(node->shape.y / CELL_SIZE);

        if (cx < 0 || cx >= cols || cy < 0 || cy >= rows)
        {
            printf("[!] Nodo fuera del grid: cx=%d, cy=%d\n", cx, cy);
            return;
        }

        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                int nx = cx + dx;
                int ny = cy + dy;

                if (nx >= 0 && nx < cols && ny >= 0 && ny < rows)
                {
                    Vector<Node *> &bucket = cells[nx][ny];
                    for (size_t i = 0; i < bucket.getSize(); ++i)
                    {
                        neighbors.push_back(bucket[i]);
                    }
                }
            }
        }
    }
};

#endif
