//
// Created by tomma on 19/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_LATTICEMESH_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_LATTICEMESH_H
#include <vector>
#include "vertex.h"
#include "utils/CreatingLooper.h"
#include "utils/FillingLooper.h"

template <int N>
class LatticeMesh {
    LatticeMesh(std::array<double, N> dim, std::array<double, N> h) {
        CreatingLooper<Vertex<N>, dim> creatingLooper;
        auto vec = creatingLooper();
        FillingLooper<dim> fillingLooper;
        fillingLooper(vec);
    }
private:
    decltype(CreatingLooper< Vertex<N>, std::make_integer_sequence<int, N>{} >()) v;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_LATTICEMESH_H
