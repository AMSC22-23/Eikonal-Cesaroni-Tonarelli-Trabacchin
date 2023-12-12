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

    explicit EikonalSolver(Mesh<D>& mesh) : mesh(mesh) {};

    virtual void solve() = 0;

    virtual std::vector<double>& getSolutions() = 0;

    virtual ~EikonalSolver() = 0;

    void getSolutionsVTK(){
        const std::string output_file_name = "output.vtk";
        getSolutionsVTK(output_file_name);
    }

    void getSolutionsVTK(const std::string& output_file_name){
        std::ofstream output_file(output_file_name);

        // header
        output_file << "# vtk DataFile Version 3.0\n";
        output_file << "ASCII\n";
        output_file << "DATASET UNSTRUCTURED_GRID\n";
        output_file << "\n";

        // points
        output_file << "POINTS " << mesh.getNumberVertices() << " double\n";
        for(int i = 0; i < mesh.getNumberVertices(); i++){
            output_file << stringCoordinatesNode(mesh.getCoordinates(i)) << "\n";
        }
        output_file << "\n";

        // cells
        std::vector<std::array<int, N>> shapes = getMeshShapes();
        output_file << "CELLS        " << shapes.size() << " " << shapes.size() * (N + 1) << "\n";
        for(int i = 0; i < shapes.size(); i++){
            output_file << stringNodesShape(shapes[i]) << "\n";
        }
        output_file << "\n";

        // cell_type (5 for triangle N = 3, 10 for tetrahedral N = 4)
        output_file << "CELL_TYPES " << shapes.size() << "\n";
        for(int i = 0; i < shapes.size(); i++){
            unsigned int num;
            if constexpr (N == 3){
                num = 5;
            } else {
                num = 10;
            }
            output_file << num << " ";
        }
        output_file << "\n";

        // lookup_table
        output_file << "POINT_DATA " << solutions.size() << "\n";
        output_file << "SCALARS solution double 1\n";
        output_file << "LOOKUP_TABLE default\n";
        for(double solution : solutions){
            output_file << solution << " ";
        }
        output_file << "\n";

        output_file.close();
    }

protected:
    Mesh<D>& mesh;
    std::vector<double> solutions;

private:
    std::vector<std::array<int, N>> getMeshShapes(){
        std::vector<std::array<int, N>> res;
        std::array<int, N> s;
        for(int i = 0; i < mesh.getNumberVertices(); i++){
            std::vector<int> shapes = mesh.getShapes(i);
            //TO-FINISH
            unsigned int flag = 0;

            for(int k = 0; k < shapes.size() - 1; k += N - 1){
                for(int j = 0; j < i; j++){
                    for(int m = 0; m < N-1; m++){
                        if(shapes[k + m] == j)
                            flag++;
                    }
                }
                if(flag == 0){
                    s[0] = i;
                    for(int j = 1; j < N; j++){
                        s[j] = shapes[j - 1 + k];
                    }
                    res.emplace_back(s);
                }
                s.clear();
            }

        }
        return res;
    }

    std::string stringCoordinatesNode(std::array<double, D> coordinates){
        std::string res;
        for(int i = 0; i < D; i++){
            res.append(coordinates[i] + " ");
        }
        return res;
    }

    std::string stringNodesShape(std::array<int, N> shape){
        std::string res;
        res.append(N + "   ");
        for(int i = 0; i < N; i++){
            res.append(shape[i] + "  ");
        }
        return res;
    }

};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_EIKONALSOLVER_H
