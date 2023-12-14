//
// Created by sabri on 23/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGLEMESH_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGLEMESH_H
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <set>
#include <math.h>
#include <algorithm>
#include <tuple>
#include <cassert>
#include <numeric>
#include "Mesh.h"

template<int D>
class TriangularMesh : public Mesh<D>{
public:

    TriangularMesh(const std::string& mesh_file_path) : Mesh<D>() {
        std::vector<std::set<int>> sets = Mesh<D>::init_mesh(mesh_file_path, 3);

        int cont = 0;
        for(int i=0; i < this->getNumberVertices(); i++) {
            this->ngh[i] = cont;
            for (const auto &x: sets[i]) {
                std::vector<int> tmp(std::min(sets[i].size(), sets[x].size()), 0);
                std::vector<int>::iterator end;
                end = std::set_intersection(sets[i].begin(), sets[i].end(),
                                            sets[x].begin(), sets[x].end(),
                                            tmp.begin());
                std::vector<int>::iterator it;

                for (it = tmp.begin(); it != end; it++) {
                    if (*it > x) {
                        this->shapes.push_back(x);
                        this->shapes.push_back(*it);
                        cont += this->vertices_per_shape - 1;
                    }
                }
            }
        }
    }

};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGLEMESH_H
