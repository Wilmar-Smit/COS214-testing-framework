#include "testing.h"
template <class T>
testing<T>::testing(T *testObject, T *correctObject): testObject(testObject), correctObject(correctObject)
{
    this->passes = 0;
    this->fails=0;

    

}

template <class T>
testing<T>::~testing()
{
}

template <class T>
void testing<T>::createTestingOutline()
{
}

template <class T>
void testing<T>::equalTest()
{
}

template <class T>
void testing<T>::textCompare()
{
}
