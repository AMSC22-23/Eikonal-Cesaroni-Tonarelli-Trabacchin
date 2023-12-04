//
// Created by tomma on 04/12/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_PARALLELEIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_PARALLELEIKONALSOLVER_H
#include "Mesh.h"
template<int D, int N>
class ParallelEikonalSolver{
public:
    ParallelEikonalSolver()
private:
    Mesh<D>& mesh;
    std::vector<int>& boundary_vertices;
    std::vector<int> active_list;
    std::vector<double> solutions_in;
    std::vector<double> solutions_out;
    double velocity = 1;
};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_PARALLELEIKONALSOLVER_H
