#include "testing.h"

//############################ testing code ############################
template <class T, class J>
testing<T, J>::testing(T *testObject, J *correctObject) : testObject(testObject), correctObject(correctObject)
{
    this->passes = 0;
    this->fails = 0;

}

template <class T, class j>
testing<T, j>::~testing()
{
}

template <class T, class J>
T *testing<T, J>::getTestObj()
{
}

template <class T, class J>
J *testing<T, J>::getCorrectObj()
{
}

// ################################ Suite code ############################################
template <class T, class J>
suite<T, J>::suite(Array<string> testsToRun)
{

}
template <class T, class J>
suite<T, J>::~suite()
{

}
// prints the states upon deletion
template <class T, class J>
void suite<T, J>::textCompare()
{

}
template <class T, class J>
void suite<T, J>::testCompare(string testAgainst)
{

}
template <class T, class J>
void suite<T, J>::equalsTest()
{

}
template <class T, class J>
template <class X, class Y>
void suite<T, J>::equalsTest(X lhs, Y rhs)
{

}