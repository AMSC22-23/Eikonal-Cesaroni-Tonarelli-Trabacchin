//
// Created by Melanie Tonarelli on 11/12/23.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_EIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_EIKONALSOLVER_H

#include "Mesh.h"
#include "../localProblem_alt2/include/Eikonal_traits.hpp"

template<int D, int N>
class EikonalSolver {
public:

    virtual void solve() = 0;

    virtual std::vector<double>& getSolutions() = 0;

};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_EIKONALSOLVER_H
