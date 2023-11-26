//
// Created by tomma on 26/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H

#include <cassert>
#include <iostream>

class CircularList {
    struct Node {
        int data;
        Node* next;
        explicit Node(int data):data(data), next(nullptr) {}
    };
public:
    CircularList() {
        head = nullptr;
        prec = nullptr;
    }
    int getNext() {
        prec = safe_advance(prec);
        return prec -> data;
    }

    void add(int v) {
        Node* newNode = new Node(v);
        if(head == nullptr) {
            newNode -> next = nullptr;
            head = newNode;
            prec = newNode;
        } else {
            if(prec -> next != nullptr) {
                Node* curr = prec -> next;
                newNode -> next = curr;
                prec -> next = newNode;
                prec = newNode;
            } else {
                newNode -> next = nullptr;
                prec -> next = newNode;
                prec = newNode;
            }
        }
    }

    void remove() {
        assert(head != nullptr);
        if(head -> next == nullptr) {//the list contains only one element
            delete head;
            head = nullptr;
            prec = nullptr;
        } else {
            if(safe_advance(prec) == head) { //head is to be eliminated, and it is not the only element in the list
                Node* newHead = head -> next;
                delete head;
                head = newHead;
            } else {
                if((prec -> next) -> next == nullptr) {//the last element of the list is to be eliminated, and it is not the only element in the list
                    delete prec -> next;
                    prec -> next = nullptr;
                } else {// the element to be eliminated is a middle one, and it not the only one in the list
                    Node* nodeToRemove = prec -> next;
                    prec -> next = nodeToRemove -> next;
                    delete nodeToRemove;
                }
            }

        }
    }

    bool isEmpty() {
        return head == nullptr;
    }

    bool isPresent(int v) {
        assert(head != nullptr);
        Node* curr = head;
        do {
            if(curr -> data == v) {
                return true;
            } else {
                curr = curr -> next;
            }
        }while(curr != nullptr);
        return false;
    }

    friend std::ostream& operator<<(std::ostream& os, const CircularList& other) {
        assert(other.head != nullptr);
        Node* curr = other.head;
        do {
            os << curr -> data << " ";
            curr = curr -> next;
        }while(curr != nullptr);
        os << std::endl;
        return os;
    }
private:
    Node* safe_advance(Node* node) {
        if(node -> next == nullptr) {
            return head;
        } else {
            return node -> next;
        }
    }
    Node* head;
    Node* prec;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
