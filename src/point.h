//
// Created by tomma on 17/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_POINT_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_POINT_H
#include "shape.h"
#include "vertex.h"
template <int N>
class Shape<Vertex<N>, 1> {
    Shape() = default;
    Shape(const Vertex<N>& v) : vertex(v) {};
    double solveLocalProblem() const{}
private:
    Vertex<N> &vertex;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_POINT_H
