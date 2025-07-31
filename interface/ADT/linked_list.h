#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include <stdio.h>
#include <_node.h>

template <class T>
class LinkedList {
private:
    _Node<T> *head;

public:
    LinkedList() : head(nullptr) {}
    
    LinkedList(T value) {
        head = new _Node<T>(value);
    }
    
    ~LinkedList() {
        _Node<T>* current = head;
        while (current) {
            _Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void insert(T value) {
        _Node<T> *new_node = new _Node<T>(value);
        if (!head) {
            head = new_node;
        } else {
            _Node<T> *current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;
        }
    }

    void remove(T value) {
        if (!head) return;

        if (head->value == value) {
            _Node<T> *temp = head;
            head = head->next;
            temp->next = nullptr;
            delete temp;
            return;
        }

        _Node<T> *current = head;
        while (current->next && current->next->value != value) {
            current = current->next;
        }

        if (current->next) {
            _Node<T> *temp = current->next;
            current->next = temp->next;
            temp->next = nullptr;
            delete temp;
        }
    }

    bool find(T value) {
        _Node<T> *current = head;
        while (current) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

#endif