#ifndef ARRAY_H
#define ARRAY_H

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#include <stdexcept>
using namespace std;
template <class T>
class Array
{
public:
    int length;
    Array(int length);
    ~Array();

    void deleteAll();
    void increaseSizeBy(int increaseBy);
    void setLength(int length);

    T *getIndex(int i);
    T *operator[](int i);

    void insertNewItem(T* newItem);
    

private:
    T **array;
};
#include "array.cpp"

#endif