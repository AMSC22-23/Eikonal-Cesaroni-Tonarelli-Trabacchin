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
#include<algorithm>
template<int D>
class TriangularMesh {
public:
    static constexpr int vertices_per_triangle = 3;
    TriangularMesh(const std::string& mesh_file_path){
        std::ifstream mesh_file (mesh_file_path);
        if(mesh_file.is_open()) {
            std::string buffer;
            std::getline(mesh_file, buffer);
            std::getline(mesh_file, buffer);
            std::getline(mesh_file, buffer);
            std::getline(mesh_file, buffer);
            mesh_file>>buffer;
            int vertices_number;
            mesh_file>>vertices_number;
            mesh_file>>buffer;
            std::cout << vertices_number << std::endl;
            geo.resize(vertices_number*D);
            for(int i=0; i<vertices_number*D; i++){
                mesh_file>>geo[i];
            }
            mesh_file>>buffer;
            int triangle_number;
            mesh_file>>triangle_number;
            mesh_file>>buffer;
            std::vector<std::set<int>> sets;
            sets.resize(vertices_number);
            ngh.resize(vertices_number);
            for(int i=0; i<triangle_number; i++){
                mesh_file>>buffer;
                std::array<int,vertices_per_triangle> tmp;
                for(int j=0; j<vertices_per_triangle; j++){
                    mesh_file>>tmp[j];
                }
                for(int j=0; j<3; j++){
                    for(int k=0; k<3; k++){
                        if(j!=k){
                            sets[tmp[j]].insert(tmp[k]);
                        }
                    }
                }

            }
            int cont = 0;
            for(int i=0; i<vertices_number; i++) {
                ngh[i] = cont;
                for(const auto& x: sets[i]){
                    std::vector<int> tmp (std::min(sets[i].size(), sets[x].size() ), 0);
                    std::vector<int>::iterator end;
                    end = std::set_intersection(sets[i].begin(), sets[i].end(), sets[x].begin(), sets[x].end(), tmp.begin());
                    std::vector<int>::iterator it;
                    for(it=tmp.begin(); it!=end; it++){
                        if(*it > x){
                            shapes.push_back(x);
                            shapes.push_back(*it);
                            cont += 2;
                        }
                    }
                }
            }
            mesh_file.close();
        } else {
            std::cout << "opppppsssss" << std::endl;
        }
    }

    std::string toString() {
        int cont = 0;
        int index = 0;
        std::string res = "";
       while(true) {
           res+= "vertex " + std::to_string(cont) + ": " ;
           for(int i=index; i< (cont < ngh.size()-1 ? ngh[cont+1] : shapes.size()); i+=2){
               res+=std::to_string(shapes[i]) + " " + std::to_string (shapes[i+1]) + ", ";
               index = i+2;
           }
           cont++;
           if(cont==ngh.size()){
               break;
           }
           res += "\n";
       }
       return res;
    }


//private:
    std::vector<double> geo;
    std::vector<int> shapes;
    std::vector<int> ngh;
};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGLEMESH_H
