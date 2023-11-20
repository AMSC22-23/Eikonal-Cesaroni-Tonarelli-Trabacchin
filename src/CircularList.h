//
// Created by tomma on 19/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
#include "vertex.h"
#include <iostream>
// creating class using the class keyword
template<int N>
class CircularList {

    class Node
    {
    public:
        std::shared_ptr<Vertex<N>> data;
        Node* next = 0;

        Node(std::shared_ptr<Vertex<2>> user_data) : data(user_data) {};


    };
public:
    CircularList(){
        tail = nullptr;
        head = nullptr;
        prec = nullptr;
    }

    std::shared_ptr<Vertex<N>> getNext() {
        prec = prec->next;
        return prec->data;
    }

    void add(std::shared_ptr<Vertex<N>> v) {
        Node* node = new Node(v);
        if(isEmpty()) {
            node -> next = node;
            tail = node;
            head = node;
            prec = node;
        }
        else {
            tail->next = node;
            tail = tail->next;
            tail->next = head;
        }
    }

    void remove(){
        Node* successor = prec->next->next;
        delete prec->next;
        if(successor == prec->next){
            tail = nullptr;
            head = nullptr;
            prec = nullptr;
        } else {
            prec->next = successor;
        }
    }

    bool isEmpty(){
        return head == nullptr;
    }

    bool isPresent(std::shared_ptr<Vertex<N>> v){
        Node* tmp = tail;
        do {
            tmp = tmp->next;
            if(tmp->data->getId() == v->getId()){
                return true;
            }
        } while(tmp->next != head);
        return false;
    }

    friend std::ostream& operator<< (std::ostream& os, const CircularList<N>& list) {
        Node* tmp = list.head;
        os << "List = ";
        do {
            os << *(tmp->data) << " ";
            tmp = tmp->next;
        } while(tmp != list.head);
        os << "\n";
        return os;
    }

private:
    Node* prec;
    Node* head;
    Node* tail;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
