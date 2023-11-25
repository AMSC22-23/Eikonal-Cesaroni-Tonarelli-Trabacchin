//
// Created by tomma on 20/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
#include "vertex.h"
template <typename T>
class CircularList {
    struct Node {
        T& data;
        Node* next;
        Node(T& data) : data(data){};
    };
public:
    CircularList() {
        head = nullptr;
        tail = nullptr;
        prec = nullptr;
    }
    void add(T& v) {
        Node* newNode = new Node(v);
        if(tail != nullptr) {
            newNode -> next = head;
            tail -> next = newNode;
            if(tail == prec) {
                prec = newNode;
            }
            tail = newNode;
        } else {
            tail = newNode;
            head = newNode;
            prec = newNode;
            tail -> next = head;
        }
    }

    void remove() {
        if(head == tail) {
            delete head;
            head = nullptr;
            tail = nullptr;
            prec = nullptr;
        } else {
            Node* nodeToRemove = prec -> next;
            if(nodeToRemove == head) {
                Node* newHead = nodeToRemove -> next;
                head = newHead;
            } else if(nodeToRemove == tail) {
                Node* newTail = prec;
                head = newTail;
            }
            prec -> next = nodeToRemove -> next;
            delete nodeToRemove;
        }


    }

    T& getNext() {
        Node* nodeToReturn = prec -> next;
        prec = nodeToReturn;
        return nodeToReturn -> data;
    }

    bool isEmpty() {
        return head == tail;
    }

    bool isPresent(T& v) {
        Node* curr = head;
        do {
            if(curr == nullptr) {
                return false;
            }
            if(curr -> data == v) {
                return true;
            }
            curr = curr -> next;
        }while(curr != head && curr != nullptr);

        return false;
    }

    friend std::ostream& operator<< (std::ostream& os, const CircularList<T>& list) {
        Node* tmp = list.head;
        os << "List = ";
        do {
            os << (tmp->data) << " ";
            tmp = tmp->next;
        } while(tmp != list.head);
        os << "\n";
        return os;
    }

    ~CircularList() {
        if(head != nullptr) {
            Node* curr = head;
            Node* next;
            while(true) {
                next = curr -> next;
                delete curr;
                curr = next;
                if(curr == head) {
                    break;
                }
            }
        }
    }

private:
    Node* head;
    Node* tail;
    Node* prec;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CIRCULARLIST_H
