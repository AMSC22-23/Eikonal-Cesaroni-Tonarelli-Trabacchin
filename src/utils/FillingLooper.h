//
// Created by tomma on 20/11/2023.
//
#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_FILLINGLOOPER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_FILLINGLOOPER_H
#include <initializer_list>
#include "vertex.h"

template<int ...Ints>
struct FillingLooper;

template <int I, int ...N >
struct FillingLooper<I, N...> {
    template <int D, typename V>
    constexpr void operator()(int& cont, V& v, int x...) {
       for(int i = 0; i < I; i++) {
           FillingLooper<N...> ()(cont, v[i], x, i);
       }
   }
};

template <>
struct FillingLooper<> {
    template <int D, typename V>
    constexpr void operator()(int& cont, V& v, std::initializer_list<int> list) {
        v = Vertex<D>(list, cont);
        cont++;
    }
};
#endif