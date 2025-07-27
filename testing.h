#ifndef TESTING_H
#define TESTING_H

template <class T>
class testing
{
private:
    int passes;
    int fails;
    T *testObject;
    T *correctObject;
    // T must have the == operator overloaded with itself to check validity.
    // there will also be single value checks made as static functions for specific unit checks

public:
    testing(T *testObject, T *correctObject);
    // will also do a check to see if a testing outline has already been made into a .txt
    ~testing();
    // delets the two T's
    void createTestingOutline();
    // will run if no testing outline exist
    void equalTest();
    // runs an == test
    void textCompare();
    // prints out the toString() functions if they exist on T.
};
#include "testing.cpp"
#endif