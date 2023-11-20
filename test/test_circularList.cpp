#include "CircularList.h"
#include "vertex.h"
#include <cassert>
#include <array>
#include <iostream>

int main(){
    // We create a Circular list with (1,1) - (2,2) - (3,3) - (4,4) - (5,5) with vertices of size 2
    CircularList<2> list = CircularList<2>();
    assert(list.isEmpty());
    for(int i = 1; i <= 4; i++){
        std::array<double, 2> a({i*1., i*1.});
        std::shared_ptr<Vertex<2>> v = std::make_shared<Vertex<2>>(a,i);
        list.add(v);
        std::cout << list;
    }
    std::array<double, 2> a({1., 1.});
    std::shared_ptr<Vertex<2>> v = std::make_shared<Vertex<2>>(a, 1);
    assert(list.isPresent(v));
    std::cout << list;
    list.remove();
    std::cout << list;
    list.getNext();
    list.remove();
    std::cout << list;
    return 0;
}