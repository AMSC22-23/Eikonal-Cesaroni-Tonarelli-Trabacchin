//
// Created by tomma on 22/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_MESH_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_MESH_H
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <numeric>
#include <float.h>

template <int D>
class Mesh {

public:

    std::string toString() {
        int cont = 0;
        int index = 0;
        std::string res = "";
        while(true) {
            res+= "vertex " + std::to_string(cont) + ": " ;
            for(int i=index; i< (cont < ngh.size()-1 ? ngh[cont+1] : shapes.size()); i+= vertices_per_shape - 1){
                res+=std::to_string(shapes[i]) + " " + std::to_string(shapes[i+1]) + ", ";
                index = i + vertices_per_shape - 1;
            }
            cont++;
            if(cont==ngh.size()){
                break;
            }
            res += "\n";
        }
        return res;
    }

    int getNumberVertices() const {
        return geo.size() / D;
    };

    int getVerticesPerShape() const {
        return vertices_per_shape;
    }

    std::vector<int> getNeighbors(int vertex){
        std::set<int> neighbors;
        for(int i = ngh[vertex]; i < (vertex != ngh.size() -1 ? ngh[vertex + 1] : shapes.size()); i++){
            neighbors.insert(shapes[i]);
        }
        std::vector<int> res(neighbors.begin(), neighbors.end());
        return res;
    }

    std::vector<int> getShapes(int vertex){
        std::vector<int> shapes_v;
        for(int i = ngh[vertex]; i < (vertex != ngh.size() -1 ? ngh[vertex + 1] : shapes.size()); i++){
            shapes_v.push_back(shapes[i]);
        }
        return shapes_v;
    }

    std::array<double, D> getCoordinates(int vertex){
        std::array<double,D> coord;
        for(int i = D * vertex; i < D * vertex + D; i++){
            coord[i - D * vertex] = geo[i];
        }
        return coord;
    }

    int getMapVertex(int vertex){
        return map_vertices[vertex];
    }

    int getOriginalNumberOfVertices(){
        return map_vertices.size();
    }

    std::string getFilenameInputMesh(){
        return filename_input_mesh;
    }

    int getNearestVertex(std::array<double, D> coordinates){
        double min_distance = DBL_MAX;
        int min_vertex = 0;
        for(int i = 0; i < getNumberVertices(); i++){
            double distance = getDistance(coordinates, getCoordinates(i));
            if(distance < min_distance){
                min_distance = distance;
                min_vertex = i;
            }
        }
        return min_vertex;
    }

protected:

    double getDistance(std::array<double, D> c1, std::array<double, D> c2){
        double res = 0;
        for(int i = 0; i < D; i++){
            res += (c1[i] - c2[i]) * (c1[i] - c2[i]);
        }
        res = std::sqrt(res);
        return res;
    }

    void removeDuplicateVertices(){
        std::vector<int> pos;
        pos.resize(geo.size()/D);
        std::iota(pos.begin(), pos.end(),0);
        std::sort(pos.begin(), pos.end(), [&](std::size_t i, std::size_t j) { return verticesCompare(i,j) == 1; });

        int current_index = 0;
        int prec;
        std::vector<int> same;
        std::vector<double> reduced_geo;
        reduced_geo.resize(0);
        map_vertices.resize(geo.size() / D);

        while(current_index < pos.size()){
            prec=current_index;
            current_index++;
            same.push_back(pos[prec]);
            while(true){
                if( current_index < pos.size() && verticesCompare(pos[prec], pos[current_index]) == 0){
                    same.push_back(pos[current_index]);
                    current_index++;
                } else{
                    for(int j : same){
                        map_vertices[j]= (int)reduced_geo.size() / D;
                    }
                    for(int i=0; i<D; i++){
                        reduced_geo.push_back(geo[same[0]*D+i]);
                    }
                    break;
                }
            }
            same.clear();
        }
        geo = reduced_geo;
    }

    int verticesCompare(int i, int j){
        for(int k = 0; k < D; k++){
            if(geo[D * i + k] < geo[D * j + k]){
                return 1;
            } else if (geo[D * i + k] > geo[D * j + k]){
                return -1;
            }
        }
        return 0;
    }

    std::vector<std::set<int>> init_mesh(const std::string& mesh_file_path, int vertices_per_shape_) {
        filename_input_mesh = mesh_file_path;
        vertices_per_shape = vertices_per_shape_;
        std::ifstream mesh_file (mesh_file_path);
        std::vector<std::set<int>> sets;
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
            geo.resize(vertices_number*D);
            int ignore;
            for(int i = 0; i < vertices_number; i++){
                for(int j = 0; j < 3; j++){
                    if(j < D)
                        mesh_file >> geo[D * i + j];
                    else mesh_file >> ignore;
                }

            }
            removeDuplicateVertices();


            vertices_number = geo.size()/D;
            mesh_file>>buffer;
            int triangle_number;
            mesh_file>>triangle_number;
            mesh_file>>buffer;
            sets.resize(vertices_number);
            ngh.resize(vertices_number);
            for(int i=0; i<triangle_number; i++){
                mesh_file>>buffer;
                std::vector<int> tmp;
                tmp.resize(vertices_per_shape);
                for(int j=0; j<vertices_per_shape; j++){
                    mesh_file>>tmp[j];
                    tmp[j] = map_vertices[tmp[j]];
                }
                for(int j=0; j < vertices_per_shape; j++){
                    for(int k=0; k < vertices_per_shape; k++){
                        if(j!=k){
                            sets[tmp[j]].insert(tmp[k]);
                        }
                    }
                }

            }
            mesh_file.close();
        } else {
            std::cout << "Couldn't open mesh file." << std::endl;
        }
        return sets;
    }

    std::vector<double> geo;
    std::vector<int> shapes;
    std::vector<int> ngh;
    int vertices_per_shape = 0;
    std::vector<int> map_vertices;
    std::string filename_input_mesh;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_MESH_H
