#ifndef TESTING_H
#define TESTING_H
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
class testing
{
private:
    int passes;
    int fails;
    T *testObject; // used for the start of all tests as a base
    // make a new test object copy
    J *correctObject; // used for the start of all tests as a base
    // make a new test object copy

    Array<Suite<T, J>> *testSuites;
    // T must have the == operator overloaded with itself to check validity.
    // there will also be single value checks made as static functions for specific unit checks

public:
    testing(T testObject, J correctObject);
    ~testing();
    T *getTestObj();
    J *getCorrectObj();

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
    Suite(Suite<T,J> & copy);
    ~Suite();
    // prints the states upon deletion

    void runTests(Array<string> testsToRun);
    void textCompare();
    void testCompare(string testAgainst);

    void equalsTest();

    template <class X, class Y>
    static void equalsTest(X lhs, Y rhs);

    T *getTestObj();
    J *getCorrectObj();
    void setTest(T *testObj);
    void setCorrect(T *corrObj);

    Suite<T, J> &operator=(Suite<T, J> &copy);

    string printGreen(int &index, string tstString, string corString);
    string printRed(int &index, string tstString, string corString);
};

#include "testing.cpp"
#endif