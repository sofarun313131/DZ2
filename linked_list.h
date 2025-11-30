#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Node {
    string val;
    Node* next;
    Node* prev;

    Node(const string& v);
};

class LinkedList {
    Node* head;
    Node* tail;
    size_t cnt;
    bool doubly;
    bool circular;

public:
    LinkedList(bool isDoubly=false, bool isCircular=false);
    ~LinkedList();
    bool empty() const;
    size_t size() const;
    bool isDoubly() const;
    bool isCircular() const;
    string typeSymbol() const;
    void append(const string& value);
    void prepend(const string& value);
    void print(ostream& os = cout) const;
    vector<size_t> findAll(const string& target) const;
    bool removeFirst(const string& target);
    size_t removeAll(const string& target);
    void clear();
    bool saveToFile(const string& filename) const;
    vector<string> toVector() const;
    void stealFrom(LinkedList* other);
    static LinkedList* fromTypeAndElems(int t, const vector<string>& elems);
    static LinkedList* loadFromFile(const string& filename, string& error);
};

vector<string> splitByCommaPreserve(const string& s);
string trim(const string& s);
bool parseArgs(int argc, char* argv[],
               int& t_val, int& c_val,
               vector<string>& elems,
               string& err);
void interactiveMenu(LinkedList* list);
#endif
