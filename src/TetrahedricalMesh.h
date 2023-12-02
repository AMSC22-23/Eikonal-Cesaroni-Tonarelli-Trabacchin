//
// Created by Melanie Tonarelli on 28/11/23.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TETRAHEDRICALMESH_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TETRAHEDRICALMESH_H
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
class TetrahedricalMesh : public Mesh<D>{
public:

    TetrahedricalMesh(const std::string& mesh_file_path) : Mesh<D>() {
        std::vector<std::set<int>> sets = Mesh<D>::init_mesh(mesh_file_path, 4);
        int cont = 0;
        for(int i=0; i < this->getNumberVertices(); i++) {
            Mesh<D>::ngh[i] = cont;
            for(const auto& x: sets[i]){
                std::vector<int> tmp (std::min(sets[i].size(), sets[x].size() ), 0);
                std::vector<int>::iterator end;
                end = std::set_intersection(sets[i].begin(), sets[i].end(),
                                            sets[x].begin(), sets[x].end(),
                                            tmp.begin());
                std::vector<int>::iterator it;
                std::set<int> current_set;

                for(it=tmp.begin(); it!=end; it++){
                    current_set.insert(*it);
                }

                for(it = tmp.begin(); it != end; it++){
                    if(*it > x) {
                        std::vector<int>::iterator end2;
                        std::vector<int> tmp2(std::min(current_set.size(), sets[*it].size()), 0);
                        std::vector<int>::iterator it2;
                        end2 = std::set_intersection(current_set.begin(), current_set.end(),
                                                     sets[*it].begin(), sets[*it].end(),
                                                     tmp2.begin());
                        for(it2 = tmp2.begin(); it2 != end2; it2++){
                            if(*it2 > *it){
                                this->shapes.push_back(x);
                                this->shapes.push_back(*it);
                                this->shapes.push_back(*it2);
                                cont += this->vertices_per_shape - 1;
                            }
                        }

                    }
                }
            }
        }
    }


};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TETRAHEDRICALMESH_H
