#include "array.h"

template <class T>
Array<T>::Array(int length)
{
    array = new T[length]();
    this->length = length;
}
template <class T>
Array<T>::Array(Array<T> &copy)
{
    array = new T[copy.length]();
    this->length = copy.length;

    for (int i = 0; i < length; i++)
    {
        array[i] = copy[i];
    }
}

template <class T>
Array<T>::~Array()
{
    delete[] array;
}

template <class T>
void Array<T>::deleteAll()
{
}

template <class T>
void Array<T>::increaseSizeBy(int increaseBy)
{
    if (increaseBy > 0)
    {
        int newLength = length + increaseBy;
        T *n_array = new T[newLength]();

        for (int i = 0; i < length; i++)
        {
            n_array[i] = array[i];
        }

        delete[] array;

        array = n_array;
        length = newLength;
    }
    else
        throw out_of_range("Array cannot have its size increased by negative number");
}

template <class T>
void Array<T>::setLength(int newLength)
{
    // will delete all data inside the array
    if (newLength > 0)
    {
        delete[] array;
        array = new T[newLength];
        for (int i = 0; i < newLength; i++)
        {
            array[i] = NULL;
        }

        length = newLength;
    }
    else
        throw out_of_range("Array length must be greater than 0");
}

template <class T>
T &Array<T>::getIndex(int i)
{
    if (i >= 0 && i < length)
    {
        return array[i];
    }
    else
    {
        throw out_of_range("Array index cannot be less than 0 or greater than length");
    }
}
template <class T>
T &Array<T>::operator[](int i)
{
    if (i >= 0 && i < length)
    {
        return array[i];
    }
    else
    {
        throw out_of_range("Array index cannot be less than 0 or greater than length");
    }
}
template <class T>
const T &Array<T>::operator[](int i) const
{
    if (i >= 0 && i < length)
    {
        return array[i];
    }
    else
    {
        throw out_of_range("Array index cannot be less than 0 or greater than length");
    }
}

template <class T>
Array<T> &Array<T>::operator=(const Array<T> rhs)
{
    if (this == &rhs)
    {
        return *this;
    }
    this->length = rhs.length;
    delete[] array;
    array = new T[length];
    for (int i = 0; i < rhs.length; i++)
    {
        array[i] = rhs[i];
    }

    return *this;
}

template <class T>
bool Array<T>::operator==(const Array<T> rhs)
{
    if (length != rhs.length)
        return false;

    for (int i = 0; i < rhs.length;i++)
    {
        if (array[i] != rhs[i])
            return false;
    }

    return true;
}

template <class T>
void Array<T>::insertNewItem(T newItem)
{
    bool foundSpace = false;
    for (int i = 0; i < length; i++)
    {
        if (array[i] == T())
        {
            array[i] = newItem;
            foundSpace = true;
            break;
        }
    }
    if (foundSpace)
        return;
    else
    {
        increaseSizeBy(1);
        array[length - 1] = newItem;
    }
}

template <class T>
int Array<T>::getLength() const
{
    return length;
}

template <class T>
string to_string(Array<T> array)
{
    string obj = "[ ";
    for (int i = 0; i < array.getLength(); i++)
    {

        obj += to_string(array[i]);
        if (i != array.getLength() - 1)
            obj += ", ";
    }

    obj += " ]";
    return obj;
}
