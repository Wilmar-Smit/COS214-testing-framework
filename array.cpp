#include "array.h"

template <class T>
Array<T>::Array(int length)
{
    array = new T *[length];
    this->length = length;
}
template <class T>
Array<T>::Array(T *items, int length)
{
    this->length = length;
    array = new T *[length];
    for (int i = 0; i < length; i++)
    {
        array[i] = new T(items[i]);
        // call copy constructor
    }
}

template <class T>
Array<T>::~Array()
{
    deleteAll();
    delete[] array;
}

template <class T>
void Array<T>::deleteAll()
{
    for (int i = 0; i < length; i++)
    {
        delete array[i];
        array[i] = NULL;
    }
}

template <class T>
void Array<T>::increaseSizeBy(int increaseBy)
{
    if (increaseBy > 0)
    {
        int newLength = length + increaseBy;
        T **n_array = new T *[newLength];

        for (int i = 0; i < length; i++)
        {
            n_array[i] = array[i];
        }
        for (int i = length; i < newLength; i++)
        {
            n_array[i] = NULL;
        }

        delete[] array;

        array = n_array;
        length = newLength;
    }
    else
        throw out_of_range(RED + "Array cannot have its size increased by negative number" + RESET);
}

template <class T>
void Array<T>::setLength(int newLength)
{
    // will delete all data inside the array
    if (newLength > 0)
    {
        deleteAll();
        delete[] array;
        array = new T *[newLength];
        for (int i = 0; i < newLength; i++)
        {
            array[i] = NULL;
        }

        length = newLength;
    }
    else
        throw out_of_range(RED + "Array length must be greater than 0" + RESET);
}

template <class T>
T *Array<T>::getIndex(int i)
{
    if (i >= 0 && i < length)
    {
        return array[i];
    }
    else
    {
        throw out_of_range(RED + "Array index cannot be less than 0 or greater than length" + RESET);
    }
}
template <class T>
T *Array<T>::operator[](int i)
{
    if (i >= 0 && i < length)
    {
        return array[i];
    }
    else
    {
        throw out_of_range(RED + "Array index cannot be less than 0 or greater than length" + RESET);
    }
}
template <class T>
void Array<T>::insertNewItem(T *newItem)
{
    bool foundSpace = false;
    for (int i = 0; i < length; i++)
    {
        if (array[i] == NULL)
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