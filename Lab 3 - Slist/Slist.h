#ifndef SLIST_H
#define SLIST_H

// include header file(s) needed
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

//template <typename T>
template <class T>
class slist {
private:
    struct node {
        node() { data = T(); next = NULL; }
        // node constructor
        node(const T& key) { data = key; }
        // overload < operator to return true if the nodes data is less than a given one
        bool operator<(const node& rhs) const { 
            return data < rhs.data; 
        }
        T data;
        node* next;
    };

    // smart pointer hand out from class06_sorting_5_handout, made explicitly for slist::node
    class sptr {
    public:
        sptr(node* _ptr = NULL) { ptr = _ptr; }
        bool operator< (const sptr& rhs) const {
            return *ptr < *rhs.ptr;
        }

        operator slist::node* () const { return ptr; }
    private:
        node* ptr;
    };

public:
    class iterator {
    public:
        iterator() { p = NULL; }
        T& operator*() { return p->data; }
        iterator& operator++() { p = p->next; return *this; }
        bool operator!=(const iterator& rhs) const { return p != rhs.p; }

    private:
        iterator(node* n_p) { p = n_p; }
        node* p;

        friend class slist<T>;
    };

public:
    slist();
    ~slist();

    void push_back(const T&);
    void sort(const string&);

    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(NULL); }

private:
    node* head;
    node* tail;
};

template <typename T>
slist<T>::slist() {
    head = new node();
    tail = head;
}

template <typename T>
slist<T>::~slist() {
    while (head->next != NULL) {
        node* p = head->next;
        head->next = p->next;
        delete p;
    }
    delete head;

    head = NULL;
    tail = NULL;
}

template <typename T>
void slist<T>::push_back(const T& din) {
    tail->next = new node(din);
    tail = tail->next;
}

template <typename T>
void slist<T>::sort(const string& algname) {

    // determine number of list elements
    int count = 0;
    for (iterator it = begin(); it != end(); ++it) {
        count += 1;
    }

    // set up smart pointer array called Ap
    vector<sptr> Ap;
    Ap.resize(count);

    for (unsigned int i = 0; i < count; i++) {
        Ap[i] = head->next;
        head->next = head->next->next;
    }

    // if mergesort, apply std::stable_sort(...)
    if (algname == "-mergesort") {
        std::stable_sort(Ap.begin(), Ap.end());
    }
    // if quicksort, apply std::sort(...)
    else if (algname == "-quicksort") {
        std::sort(Ap.begin(), Ap.end());
    }

    // use sorted Ap array to relink list 
    node* n = head;
    for (unsigned int i = 0; i != count; i++) {
        n->next = Ap[i];
        n = n->next;
        n->next = NULL;
    }
}

#endif // SLIST_H
