//
// Created by tomma on 19/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_LATTICEMESH_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_LATTICEMESH_H
#include <vector>
#include "vertex.h"
template <int N>
class LatticeMesh {
    LatticeMesh(std::array<double, N> dim) {

    }
private:
    std::vector<Vertex<N>> vertices;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_LATTICEMESH_H
