//
// Created by tomma on 19/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
#include "vertex.h"
// creating class using the class keyword
template<int N>
class CircularList {

    class Node
    {
    public:
        Vertex<N>& data = 0;
        Node* next = 0;

        Node(const Vertex<N>& user_data) : data(user_data) {}

    };
public:
    CircularList(){
        tail = nullptr;
        head = nullptr;
        prec = nullptr;
    }

    Vertex<N>& getNext() {
        prec = prec->next;
        return prec;
    }

    void add(Vertex<N> v) {
        Node* node = new Node(v);
        if(isEmpty()) {
            node -> next = node;
            tail = node;
            head = node;
            prec = node;
        }
        else {
            tail->next = node;
            node->next = &head;
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

    bool isPresent(const Vertex<N>& v){
        Node* tmp = tail;
        do {
            tmp = tmp->next;
            if(tmp->data.getId() == v.getId()){
                return true;
            }
        } while(tmp->next != head);
        return false;
    }

private:
    Node* prec;
    Node* head;
    Node* tail;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
