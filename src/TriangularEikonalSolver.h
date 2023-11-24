//
// Created by tomma on 24/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGULAREIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGULAREIKONALSOLVER_H

#include "TriangularMesh.h"
#include "CircularList.h"
#include <float.h>

template<int D>
class TriangularEikonalSolver {

public:
    TriangularEikonalSolver(TriangularMesh<D>& mesh, std::vector<int>& boundary_vertices) :
            mesh(mesh), boundary_vertices(boundary_vertices) {
        solutions_out.resize(mesh.getNumberVertices(), DBL_MAX);
        solutions_in.resize(mesh.getNumberVertices(), DBL_MAX);
        for(auto bv : boundary_vertices){
            solutions_out[bv] = 0;
            solutions_in[bv] = 0;
        }
    };

    void solver(){
        for(auto bv : boundary_vertices){
            for(auto neighbor : mesh.getNeighbors(bv)){
                active_list.add(neighbor);
            }
        }

        while(!active_list.isEmpty()){
            int v = active_list.getNext();

        }

    }

    std::vector<double>& getSolutions(){
        return solutions_out;
    }

private:
    TriangularMesh<D>& mesh;
    std::vector<int>& boundary_vertices;
    CircularList<int> active_list;
    std::vector<double> solutions_in;
    std::vector<double> solutions_out;
    double velocity = 1;

};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGULAREIKONALSOLVER_H
