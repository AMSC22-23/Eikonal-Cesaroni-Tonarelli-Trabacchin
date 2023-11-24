//
// Created by tomma on 24/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGULAREIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGULAREIKONALSOLVER_H

#include "TriangularMesh.h"

template<int D>
class TriangularEikonalSolver {

public:
    TriangularEikonalSolver(TriangularMesh<D>& mesh, std::vector<int>& boundary_vertices) :
            mesh(mesh), boundary_vertices(boundary_vertices) {};

private:
    TriangularMesh<D>& mesh;
    std::vector<int>& boundary_vertices;
    std::vector<double> solutions_in;
    std::vector<double> solutions_out;
    double velocity = 1;

};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGULAREIKONALSOLVER_H
