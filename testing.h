#ifndef TESTING_H
#define TESTING_H
#include <iostream>
#include <fstream>
#include <string>
#include "array.h"
using namespace std;

template <class T, class J>
class suite;

template <class T, class J>
class testing
{
private:
    int passes;
    int fails;
    T *testObject; // used for the start of all tests as a base
    J *correctObject; //used for the start of all tests as a base 

    // T must have the == operator overloaded with itself to check validity.
    // there will also be single value checks made as static functions for specific unit checks

public:
    testing(T *testObject, J *correctObject);
    ~testing();
    T *getTestObj();
    J *getCorrectObj();

    Array<suite> *testSuites;
};

template <class T, class J>
class suite
{
private:
    int passes;
    int fails;

    int suiteNum;
    int testsNeeded;

    Array<T> *statesTestObj;
    Array<J> *statesCorrectObj;

public:
    suite(Array<string> testsToRun);
    ~suite();
    // prints the states upon deletion

    void textCompare();
    void testCompare(string testAgainst);

    void equalsTest();
    //
    template <class X, class Y>
    static void equalsTest(X lhs, Y rhs);
};

#include "testing.cpp"
#endif