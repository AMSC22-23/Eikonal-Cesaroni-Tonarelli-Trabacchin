//
// Created by tomma on 01/12/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_SERIALEIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_SERIALEIKONALSOLVER_H
#define eikonal_tol 1e-8

#include "Mesh.h"
#include "DoubleCircularList.h"
#include <float.h>
#include "../localProblem_alt2/include/Phi.hpp"
#include "../localProblem_alt2/include/solveEikonalLocalProblem.hpp"
template<int D, int N>// D = physical dimension, N = number of vertices per shape. Combination allowed : (2,3), (3,3), (3,4)
class SerialEikonalSolver {

public:
    SerialEikonalSolver(Mesh<D>& mesh, std::vector<int>& boundary_vertices, typename Eikonal::Eikonal_traits<D,N - 2>::MMatrix M) :
            mesh(mesh), boundary_vertices(boundary_vertices), velocity{M} {
        solutions.resize(mesh.getNumberVertices(), 1000);
        for(auto bv : boundary_vertices) {
            solutions[bv] = 0;
        }
    }

    void solve(){
        int count = 0;
        for(auto bv : boundary_vertices){
            for(auto neighbor : mesh.getNeighbors(bv)){
                active_list.add(neighbor);
            }
        }
        while(!active_list.isEmpty()){
            Node* node = active_list.getNext();
            int v = node -> data;
            double old_solution = solutions[v];
            double new_solution = update(v);
            solutions[v] = new_solution;

            if(std::abs(old_solution - new_solution) < eikonal_tol) {
                //std::cout << "vertex " << v << " has converged" << std::endl;
                std::vector<int> v_neighbours = mesh.getNeighbors(v);
                for (auto b: v_neighbours) {
                    if (!active_list.isPresent(b)) {
                        double old_solution_b = solutions[b];
                        double new_solution_b = update(b);
                        if (old_solution_b > new_solution_b) {
                            solutions[b] = new_solution_b;
                            active_list.add(b);
                        }
                    }
                }
                active_list.remove(node);
            }
        }
    }

    std::vector<double>& getSolutions(){
        return solutions;
    }

protected:

    double update(int vertex) {
        std::vector<int> triangles = mesh.getShapes(vertex);
        std::vector<double> sol;
        int number_of_vertices = mesh.getVerticesPerShape();
        sol.resize(triangles.size() / D, DBL_MAX);
        for(int i = 0; i < triangles.size(); i += number_of_vertices - 1){
            std::array<std::array<double, D>, N> coordinates;
            std::array<double, N - 1> solutions_base;
            for(int j = 0; j < number_of_vertices - 1; j++) {
                coordinates[j] = mesh.getCoordinates(triangles[i + j]);
            }
            coordinates[number_of_vertices - 1] = mesh.getCoordinates(vertex);

            for(int j = 0; j < number_of_vertices - 1; j++) {
                solutions_base[j] = sol[triangles[i + j]];
            }
            sol.push_back(solveLocalProblem(coordinates, solutions_base));
        }

        double min = *std::min_element(sol.begin(), sol.end());
        return min;
    }

    double solveLocalProblem(std::array<std::array<double, D>, N> coordinates, std::array<double, N - 1> solutions_base){
        using VectorExt = typename Eikonal::Eikonal_traits<D, N - 2>::VectorExt;
        VectorExt values;
        for(int i = 0; i < N - 1; i++) {
            values[i] = solutions_base[i];
        }
        Eikonal::SimplexData<D, N> simplex{coordinates, velocity};

        Eikonal::solveEikonalLocalProblem<N, D> localSolver{simplex,values};

        auto sol = localSolver();

        assert(sol.status == 0);
        return sol.value;
    }

    Mesh<D>& mesh;
    std::vector<int>& boundary_vertices;
    DoubleCircularList active_list;
    std::vector<double> solutions;
    typename Eikonal::Eikonal_traits<D,N - 2>::MMatrix velocity;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_SERIALEIKONALSOLVER_H
