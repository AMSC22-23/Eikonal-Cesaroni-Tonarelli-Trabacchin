//
// Created by sabri on 17/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_VERTEX_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_VERTEX_H
#include <array>
#include <cmath>
#include <numeric>
#include <iostream>
template <int N>
class Vertex {
public:
    Vertex(int id) : id(id){
        for(int i = 0; i < N; i++) {
            this->coord[i] = 0.0;
        }
    }

    Vertex(const std::array<double, N> &input, int id) : id(id), coord(input) {}

    double norm() const {
        double sum = 0;
        for(int i = 0; i < N; i++) {
            sum += this->coord[i] * this->coord[i];
        }
        return sqrt(sum);
    }

    double operator* (const Vertex<N>& other) const {
        return std::inner_product(this->coord.begin(), this->coord.end(), other.coord.begin());
    }

    Vertex operator+ (const Vertex<N>& other) const {
        Vertex sum;
        for(int i = 0; i < N; i++) {
            sum.coord [i] = this->coord[i] + other.coord[i];
        }
        return sum;
    }

    Vertex operator- (const Vertex<N>& other) const {
        Vertex diff;
        for(int i = 0; i < N; i++) {
            diff.coord [i] = this->coord[i] - other.coord[i];
        }
        return diff;
    }

    double getDistance(const std::array<double, N>& coords) const {
        double sum = 0;
        for(int i = 0; i < N; i++) {
            sum += (this->coord[i] - coords[i]) * (this->coord[i] - coords[i]);
        }
        return sqrt(sum);
    }

    double operator[] (int index) const{
        return this->coord[index];
    }

    int getId() {
        return id;
    }

    friend std::ostream& operator<< (std::ostream& os, const Vertex<N>& vec) {
        os << "(";
        for(int i = 0; i < N; i++) {
            os << vec.coord[i] << ((i != N-1) ? ", " : "");
        }
        os << ")";
        return os;
    }

private:
    std::array<double, N> coord;
    int id;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_VERTEX_H
