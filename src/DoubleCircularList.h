//
// Created by tomma on 28/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_DOUBLECIRCULARLIST_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_DOUBLECIRCULARLIST_H
#include <cassert>
#include <iostream>
struct Node {
    int data;
    Node* next;
    Node* prec;
    explicit Node(int data):data(data) {next = nullptr;prec = nullptr;}
};
class DoubleCircularList {

public:
    DoubleCircularList() {
        head = nullptr;
        curr = nullptr;
    }
    Node* getNext() {
        Node* res = curr;
        curr = safe_advance(curr);
        return res;
    }

    void add(int v) {
        Node* newNode = new Node(v);
        present.insert(v);
        if(head == nullptr) {
            head = newNode,
            head -> prec = nullptr;
            head -> next = nullptr;
            curr = newNode;
        } else {
            newNode -> next = head;
            newNode -> prec = nullptr;
            head -> prec = newNode;
            head = newNode;
        }
    }

    void remove(Node* nodeToRemove) {
        present.erase(nodeToRemove->data);
        if(nodeToRemove == curr) {
            curr = curr -> next;
        }
        if(nodeToRemove -> next != nullptr && nodeToRemove -> prec != nullptr) {
            (nodeToRemove -> prec) -> next = nodeToRemove -> next;
            (nodeToRemove -> next) -> prec = nodeToRemove -> prec;
        } else if(nodeToRemove -> next != nullptr && nodeToRemove -> prec == nullptr) {//trying to remove head
            (nodeToRemove -> next) -> prec = nullptr;
            head = nodeToRemove -> next;
        } else if(nodeToRemove -> next == nullptr && nodeToRemove -> prec != nullptr) {//trying to remove the tail
            (nodeToRemove -> prec) -> next = nullptr;
        } else if(nodeToRemove -> next == nullptr && nodeToRemove -> prec == nullptr) {
            head = nullptr;
        }
        delete nodeToRemove;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    bool isPresent(int v) {
       /* if(head == nullptr) {
            return false;
        }
        Node* pNode = head;
        do {
            if(pNode -> data == v) {
                return true;
            } else {
                pNode = pNode -> next;
            }
        }while(pNode != nullptr);
        return false;*/
       return present.find(v) != present.end();
    }

    friend std::ostream& operator<<(std::ostream& os, const DoubleCircularList& other) {
        assert(other.head != nullptr);
        Node* pNode = other.head;
        do {
            os << pNode -> data << " ";
            pNode = pNode -> next;
        }while(pNode != nullptr);
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
    Node* curr;
    std::set<int> present;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_DOUBLECIRCULARLIST_H
