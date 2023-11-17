//
// Created by tomma on 17/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_SHAPE_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_SHAPE_H
#include <set>
/**
 * An abstract class representing a shape inside a mesh
 * @tparam V the Vertex
 * @tparam N
 */
template <typename V, int N>
class Shape {
    virtual double solveLocalProblem() const = 0 ;

};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_SHAPE_H
