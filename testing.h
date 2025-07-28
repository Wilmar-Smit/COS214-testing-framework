#ifndef TESTING_H
#define TESTING_H
#include <iostream>
#include <fstream>
#include <string>
#include "array.h"
using namespace std;

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

template <class T, class J>
class Suite;

template <class T, class J>
class testing
{
private:
    int passes;
    int fails;
    T *testObject;    // used for the start of all tests as a base
    J *correctObject; // used for the start of all tests as a base
    Array<Suite<T, J>> *testSuites;
    // T must have the == operator overloaded with itself to check validity.
    // there will also be single value checks made as static functions for specific unit checks

public:
    testing(T *testObject, J *correctObject);
    ~testing();
    T *getTestObj();
    J *getCorrectObj();

    void createTestSuite(string SuiteName, Array<string> testsToRun);
};

template <class T, class J>
class Suite
{
private:
    int passes, fails, testsNeeded;
    string suiteName;
    // Array<T> *statesTestObj;
    // Array<J> *statesCorrectObj;
    // requires overloading of the copy constructor or the = operator
    T* testObj;
    J* correctObj;

public:
    Suite(Array<string> testsToRun, T *testObj, J *correctObj, string suiteName = "Test");
    ~Suite();
    // prints the states upon deletion

    void textCompare();
    void testCompare(string testAgainst);

    void equalsTest();
    //
    template <class X, class Y>
    static void equalsTest(X lhs, Y rhs);

    T *getTest();
    T *getCorrectObj();
    void setTest(T *testObj);
    void setCorrect(T *corrObj);
    string printGreen(int &index, string tstString, string corString);
    string printRed(int &index, string tstString, string corString);
};

#include "testing.cpp"
#endif