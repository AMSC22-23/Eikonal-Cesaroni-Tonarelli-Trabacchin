//
// Created by tomma on 19/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_LATTICE2DSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_LATTICE2DSOLVER_H
#include "Lattice2DMesh.h"
#include <vector>
#include <float.h>

class Lattice2DSolver {
    Lattice2DSolver(const Lattice2DMesh& mesh, std::vector<int> boundary_vertices) : mesh(mesh) {
        solution.resize(mesh.getSize());
        for(const auto& v : boundary_vertices) {
            solution[v] = 0;
        }
        for(auto& v : solution) {
            if(v != 0) {
                v = DBL_MAX;
            }
        }
    }

private:
    Lattice2DMesh mesh;
    std::vector<double> solution;

};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_LATTICE2DSOLVER_H
