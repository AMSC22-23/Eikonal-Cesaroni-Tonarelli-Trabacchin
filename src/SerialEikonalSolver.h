//
// Created by tomma on 01/12/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_SERIALEIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_SERIALEIKONALSOLVER_H

#include "Mesh.h"
#include "DoubleCircularList.h"
#include <climits>
#include <algorithm>
#include <type_traits>
#include <vector>
#include "../localProblem_alt2/include/Phi.hpp"
#include "../localProblem_alt2/include/solveEikonalLocalProblem.hpp"
#include "EikonalSolver.h"

template<int D, int N>// D = physical dimension, N = number of vertices per shape. Combination allowed : (2,3), (3,3), (3,4)
class SerialEikonalSolver : public EikonalSolver<D,N> {

public:
    SerialEikonalSolver(Mesh<D>& mesh, std::vector<int>& boundary_vertices,
                        typename Eikonal::Eikonal_traits<D,N - 2>::AnisotropyM M,
                        const double tol, const double inf_value) :
            EikonalSolver<D, N>(mesh), boundary_vertices(boundary_vertices),
            velocity{M}, eikonal_tol(tol), infinity_value(inf_value)
    {
        this->solutions.resize(mesh.getNumberVertices(), infinity_value);
        for(auto bv : boundary_vertices) {
            this->solutions[bv] = 0;
        }
        active_vector.resize(this->solutions.size());
    }

    void solve(){
        std::vector<int> present;
        present.resize(this->solutions.size());
        std::fill(present.begin(), present.end() , 0);
        for(auto bv : boundary_vertices){
            for(auto neighbor : this->mesh.getNeighbors(bv)){
                active_list.add(neighbor);
                present[neighbor] = 1;
            }
        }

        while(!active_list.isEmpty()){
            Node* node = active_list.getNext();
            int v = node -> data;
            double old_solution = this->solutions[v];
            double new_solution = update(v);
            this->solutions[v] = new_solution;

            if(std::abs(old_solution - new_solution) < eikonal_tol) {
                std::vector<int> v_neighbours = this->mesh.getNeighbors(v);
                for (auto b: v_neighbours) {
                    //if (!active_list.isPresent(b)) {
                    if(present[b] == 0) {
                        double old_solution_b = this->solutions[b];
                        double new_solution_b = update(b);
                        if (old_solution_b > new_solution_b) {
                            this->solutions[b] = new_solution_b;
                            active_list.add(b);
                            present[b] = 1;
                        }
                    }
                }
                active_list.remove(node);

                //present[v] = 0;
            }
        }
    }

    void solve_vector(){
        std::vector<int> present(this->solutions.size());
        size_t active_vector_index = 0;
        size_t current_index = 0;
        std::fill(present.begin(), present.end() , 0);
        for(auto bv : boundary_vertices){
            for(auto neighbor : this->mesh.getNeighbors(bv)){
                active_vector[active_vector_index] = neighbor;
                active_vector_index++;
                present[neighbor] = 1;
            }
        }

        while(active_vector_index != 0){
            int v = active_vector[current_index];
            double old_solution = this->solutions[v];
            double new_solution = update(v);
            this->solutions[v] = new_solution;

            if(std::abs(old_solution - new_solution) < eikonal_tol) {
                std::vector<int> v_neighbours = this->mesh.getNeighbors(v);
                for (auto b: v_neighbours) {
                    if(present[b] == 0) {
                        double old_solution_b = this->solutions[b];
                        double new_solution_b = update(b);
                        if (old_solution_b > new_solution_b) {
                            this->solutions[b] = new_solution_b;
                            active_vector[active_vector_index] = b;
                            active_vector_index++;
                            present[b] = 1;
                        }
                    }
                }
                std::shift_left(begin(active_vector) + current_index, end(active_vector), 1);
                active_vector_index--;
                present[v] = 0;
            }
            if(current_index >= active_vector_index){
                current_index = 0;
            }
        }
    }

    std::vector<double> getSolutions(){
        return this->solutions;
    }

protected:

    double update(int vertex) {
        std::vector<int> triangles = this->mesh.getShapes(vertex);
        std::vector<double> sol;
        int number_of_vertices = this->mesh.getVerticesPerShape();
        sol.resize(triangles.size() / D, std::numeric_limits<double>::max());
        for(size_t i = 0; i < triangles.size(); i += number_of_vertices - 1){
            std::array<std::array<double, D>, N> coordinates;
            std::array<double, N - 1> solutions_base;
            for(int j = 0; j < number_of_vertices - 1; j++) {
                coordinates[j] = this->mesh.getCoordinates(triangles[i + j]);
            }
            coordinates[number_of_vertices - 1] = this->mesh.getCoordinates(vertex);

            for(int j = 0; j < number_of_vertices - 1; j++) {
                solutions_base[j] = this->solutions[triangles[i + j]];
            }
            //this->reorder_solutions(coordinates, solutions_base);
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

        //assert(sol.status == 0);
        return sol.value;
    }

    std::vector<int>& boundary_vertices;
    DoubleCircularList active_list;
    std::vector<int> active_vector;
    typename Eikonal::Eikonal_traits<D,N - 2>::AnisotropyM velocity;
    const double eikonal_tol;
    const double infinity_value;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_SERIALEIKONALSOLVER_H
