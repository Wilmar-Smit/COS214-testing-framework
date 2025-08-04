#ifndef Testing_H
#define Testing_H
#include <iostream>
#include <string>
#include "array.h"
using namespace std;

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

/*
All classes put into T and J must have overloads of
==
copy constructor
string to_string(T obj)
*/

template <class T, class J>
class Suite;

string to_string(string obj)
{
    return obj;
}

template <class T, class J>
class Testing
{
private:
    T *testObject; // used for the start of all tests as a base
    // make a new test object copy
    J *correctObject; // used for the start of all tests as a base
    // make a new test object copy

    Array<Suite<T, J>> *testSuites;
    // T must have the == operator overloaded with itself to check validity.
    // there will also be single value checks made as static functions for specific unit checks

public:
    Testing(T testObject, J correctObject);
    ~Testing();
    T *getTestObj();
    J *getCorrectObj();
    Suite<T, J> *getSuite(int i);
    void createTestSuite(Array<string> testsToRun, string suiteName = "Test");
};

template <class T, class J>
class Suite
{
private:
    int passes, fails;
    string suiteName;

    T *testObj;
    J *correctObj;
    // copy of the pointers made initially

public:
    Suite(Array<string> testsToRun, T *testObj, J *correctObj, string suiteName = "Test");
    Suite(Array<string> testsToRun, T testObj, J correctObj, string suiteName = "Test");
    Suite(Suite<T, J> &copy);
    ~Suite();
    // prints the states upon deletion

    void runTests(Array<string> testsToRun);
    void textCompare();

    template <class X, class Y>
    void textCompare(X &lhs, Y &rhs);

    void equalsTest();
    template <class X, class Y>
    void equalsTest(X &lhs, Y &rhs);

    T *getTestObj();
    J *getCorrectObj();
    void setTest(T *testObj);
    void setCorrect(J *corrObj);

    Suite<T, J> &operator=(Suite<T, J> &copy);

    static string printGreen(int &index, string tstString, string corString);
    static string printRed(int &index, string tstString, string corString);
};

#include "testing.cpp"
#endif