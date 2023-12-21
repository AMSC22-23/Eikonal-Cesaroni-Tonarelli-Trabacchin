//
// Created by tomma on 07/12/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_PARALLELEIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_PARALLELEIKONALSOLVER_H
//
// Created by tomma on 04/12/2023.
//

#include "Mesh.h"
#include "DoubleCircularList.h"
#include "EikonalSolver.h"
#include <climits>
#include "../localProblem_alt2/include/Phi.hpp"
#include "../localProblem_alt2/include/solveEikonalLocalProblem.hpp"
#include "omp.h"
#include <algorithm>

template<int D, int N>
class ParallelEikonalSolver : public EikonalSolver<D,N> {
public:
    ParallelEikonalSolver(Mesh<D>& mesh, std::vector<int>& boundary_vertices,
                          typename Eikonal::Eikonal_traits<D,N - 2>::AnisotropyM M,
                          int threads_number, const double tol, const double inf_value) :
            EikonalSolver<D,N>(mesh), boundary_vertices(boundary_vertices),
            threads_number(threads_number), velocity{M}, eikonal_tol(tol), infinity_value(inf_value)
    {
        this->solutions.resize(mesh.getNumberVertices(), infinity_value);
        for(auto bv : boundary_vertices)
            this->solutions[bv] = 0;
    }

    void solve(){
        std::vector<int> present(this->solutions.size());
        std::fill(present.begin(), present.end(), 0);
        int ACTIVE_LIST_LENGTH = this->solutions.size();
        active_list.resize(ACTIVE_LIST_LENGTH);
        int activeListIndex = 0;
        for(auto bv : boundary_vertices){
            for(auto neighbor : this->mesh.getNeighbors(bv)){
                active_list[activeListIndex] = neighbor;
                present[neighbor] = 1;
                activeListIndex++;
            }
        }

        #pragma omp parallel default(none) shared(present, std::cout, active_list, EikonalSolver<D,N>::solutions, activeListIndex, ACTIVE_LIST_LENGTH) num_threads(threads_number)
        {
            int address;
            std::map<int,double> vertex_to_solution;
            //std::vector<double> vertex_to_solution;
            //vertex_to_solution.resize(this->solutions.size());

            DoubleCircularList local_list;

            int thread_id = omp_get_thread_num();

            while(activeListIndex > 0) {
                for(int i=thread_id; i<activeListIndex; i+=threads_number){
                    local_list.add(active_list[i]);
                }
                #pragma omp barrier
                if(thread_id == threads_number - 1) {
                    activeListIndex = 0;
                    //std::fill(present.begin() + thread_id*present.size()/threads_number, present.end() , 0);
                }
                else {
                    //std::fill(present.begin() + thread_id*present.size()/threads_number, present.begin() + (thread_id + 1)*present.size()/threads_number, 0);
                }

                #pragma omp barrier
                vertex_to_solution.clear();
                while (!local_list.isEmpty()) {
                    Node *node = local_list.getNext();
                    int v = node->data;
                    double old_solution;
                    old_solution = getLocalSolution(v, vertex_to_solution);
                    double new_solution = update(v, vertex_to_solution);
                    address = v;
                    writeLocalSolutionEfficient(address, new_solution);
                    vertex_to_solution[v] = new_solution;
                    if (std::abs(old_solution - new_solution) < eikonal_tol) {
                        std::vector<int> v_neighbours = this->mesh.getNeighbors(v);
                        for (auto b: v_neighbours) {
                            int isPresent;
                            #pragma omp atomic capture relaxed
                            isPresent = present[b]++;
                            if (isPresent == 0) {
                                double old_solution_b;
                                old_solution_b = getLocalSolution(b, vertex_to_solution);
                                double new_solution_b = update(b, vertex_to_solution);
                                if (old_solution_b > new_solution_b) {
                                    address = b;
                                    writeLocalSolutionEfficient(address, new_solution_b);
                                    vertex_to_solution[b] = new_solution_b;
                                    int oldIndex;
                                    #pragma omp atomic capture relaxed
                                    oldIndex = activeListIndex++;
                                    active_list[oldIndex] = b;
                                }
                            }
                        }
                        local_list.remove(node);
                    }
                }
                #pragma omp barrier
            }
        }
    }

    std::vector<double> getSolutions() {
        std::vector<double> sol;
        sol.resize(this->solutions.size());
        for(size_t i = 0; i < sol.size(); i++) {
            sol[i] = this->solutions[i];
        }
        return sol;
    }

private:
    std::vector<int>& boundary_vertices;
    std::vector<int> active_list;
    int threads_number;
    typename Eikonal::Eikonal_traits<D,N - 2>::AnisotropyM velocity;
    const double eikonal_tol;
    const double infinity_value;

    void writeLocalSolution(int address, double value) {
        #pragma omp atomic write relaxed
        this->solutions[address] = value;

    }

    void writeLocalSolutionEfficient(int address, double value) {
        double snap;
        snap = this->solutions[address];
        do{

            if(snap < value) {
                break;
            }
        }while(!__atomic_compare_exchange(&this->solutions[address],&snap, &value, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED));
    }

    double getLocalSolution(int vertex, const std::map<int, double>& vertex_to_solution){
        auto it = vertex_to_solution.find(vertex);
        if(it != vertex_to_solution.end()){
            return it->second;
        } else {
            double res;
            int address = vertex;
            #pragma omp atomic read relaxed
            res = this->solutions[address];
            return res;
        }
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

        //if(sol.status !=0) {
            //std::cout << "no convergence: " << sol.status<<std::endl;
        //}
        //assert(sol.status == 0);
        return sol.value;
    }

    //sort the solutions to be sure u3 > u2 > u1
    double update(int vertex, const std::map<int, double>& vertex_to_solution) {
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
                solutions_base[j] = getLocalSolution(triangles[i+j], vertex_to_solution);
            }
            this->reorder_solutions(coordinates, solutions_base);
            sol.push_back(solveLocalProblem(coordinates, solutions_base));
        }

        double min = *std::min_element(sol.begin(), sol.end());
        return min;
    }

};


#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_PARALLELEIKONALSOLVER_H
