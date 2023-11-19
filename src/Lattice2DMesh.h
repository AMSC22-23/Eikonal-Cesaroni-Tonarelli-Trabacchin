//
// Created by tomma on 19/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_2DLATTICEMESH_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_2DLATTICEMESH_H
#include <vector>
#include <array>
#include <memory>
#include <exception>

#include <float.h>
#include "vertex.h"
class Lattice2DMesh {
public:
    Lattice2DMesh(std::array<double, 2> dim) {
        int cont = 0;
        for(double x = 0; x < dim[0]; x++) {
            std::vector<Vertex<2>> row;
            for(double y = 0; y < dim[1]; y++) {
                Vertex<2> v ({x,y}, cont);
                cont++;
                row.emplace_back(v);
            }
            vertices.emplace_back(row);
        }
        size = cont + 1;
    }

    Vertex<2>& getNearest(std::array<double,2> coord) {
        if(vertices.size() == 0) {
            throw 1;
        }
        double minDist = DBL_MAX;
        int minVertexRow = -1;
        int minVertexColumn = -1;
        for(int i = 0; i < vertices.size(); i++) {
            for(int j = 0; j < vertices[i].size(); j++) {
                double dist = vertices[i][j].getDistance(coord);
                if (dist < minDist) {
                    minVertexRow = i;
                    minVertexColumn = j;
                    minDist = dist;
                }
            }
        }
        return vertices[minVertexRow][minVertexColumn];
    }

    std::array<std::shared_ptr<Vertex<2>>, 4> getNeighbors(int id) {
        int row = id / vertices[0].size();
        int column = id % vertices.size();
        std::shared_ptr<Vertex<2>> same = std::make_shared<Vertex<2>>(vertices[row][column]);
        std::array<std::shared_ptr<Vertex<2>>, 4> neighbors;
        if(row != 0) {
            neighbors[0] = std::make_shared<Vertex<2>>(vertices[row - 1][column]);
        }
        else {
            neighbors[0] = same;
        }
        if(row != vertices.size() - 1) {
            neighbors[1] = std::make_shared<Vertex<2>>(vertices[row + 1][column]);
        }
        else {
            neighbors[1] = same;
        }

        if(column != vertices[row].size() - 1) {
            neighbors[2] = std::make_shared<Vertex<2>>(vertices[row][column + 1]);
        }
        else {
            neighbors[2] = same;
        }

        if(column != 0) {
            neighbors[3] = std::make_shared<Vertex<2>>(vertices[row][column - 1]);
        }
        else {
            neighbors[3] = same;
        }

        return neighbors;

    }

    int getSize() const {
        return size;
    }

private:
    std::vector<std::vector<Vertex<2>>> vertices;
    int size;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_2DLATTICEMESH_H
