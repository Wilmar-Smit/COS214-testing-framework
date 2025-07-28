#ifndef ARRAY_H
#define ARRAY_H


#include <stdexcept>
using namespace std;
template <class T>
class Array
{
public:
    int length;
    Array(int length);
    Array(Array<T>& copy);
    ~Array();

    void deleteAll();
    void increaseSizeBy(int increaseBy);
    void setLength(int length);

    T &getIndex(int i);
    T &operator[](int i);

    void insertNewItem(T &newItem);

private:
    T *array;
};
#include "array.cpp"

#endif