#ifndef ARRAY_H
#define ARRAY_H

#include <stdexcept>
#include <string>
using namespace std;
template <class T>

class Array
{
public:
    Array(int length);
    Array(Array<T> &copy);
    ~Array();

    void deleteAll();
    void increaseSizeBy(int increaseBy);
    void setLength(int length);

    T *getIndex(int i);
    T *operator[](int i);
    const T *operator[](int i) const;

    Array<T> &operator=(const Array<T> &rhs);
    bool operator==(const Array<T> &rhs);

    int getLength() const;
    void insertNewItem(T &newItem);

private:
    T **array;
    int length;
};

template <class T>
string to_string(Array<T> array);

#include "array.cpp"

#endif