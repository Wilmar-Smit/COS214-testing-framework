#include "testing.h"

// ############################ Testing code ############################
template <class T, class J>
Testing<T, J>::Testing(T testObject, J correctObject)
{
    this->testObject = new T(testObject);
    this->correctObject = new J(correctObject);
    testSuites = new Array<Suite<T, J>>(0);
}

template <class T, class j>
Testing<T, j>::~Testing()
{
    delete testObject;
    delete correctObject;
    delete testSuites;
}

template <class T, class J>
T *Testing<T, J>::getTestObj()
{
    return testObject;
}

template <class T, class J>
J *Testing<T, J>::getCorrectObj()
{
    return correctObject;
}
template <class T, class J>
void Testing<T, J>::createTestSuite(Array<string> testsToRun, string suiteName)
{
    Suite<T, J> nSuite(testsToRun, testObject, correctObject, suiteName);
    testSuites->insertNewItem(nSuite);
}
template <class T, class J>
Suite<T, J> *Testing<T, J>::getSuite(int i)
{
    return testSuites[i];
}

// ################################ Suite code ############################################
template <class T, class J>
Suite<T, J>::Suite(Array<string> testsToRun, T *testObj, J *correctObj, string suiteName)
{

    this->passes = 0;
    this->fails = 0;
    this->testObj = new T(*testObj);
    this->correctObj = new J(*correctObj);
    this->suiteName = suiteName;
    runTests(testsToRun);
}
template <class T, class J>
Suite<T, J>::Suite(Array<string> testsToRun, T testObj, J correctObj, string suiteName)
{

    this->passes = 0;
    this->fails = 0;
    this->testObj = new T(testObj);
    this->correctObj = new J(correctObj);
    this->suiteName = suiteName;

    runTests(testsToRun);
}
template <class T, class J>
Suite<T, J>::Suite(Suite<T, J> &copy)
{
    passes = copy.passes;
    fails = copy.fails;
    testObj = new T(*copy.testObj);
    correctObj = new J(*copy.correctObj);
}

template <class T, class J>
void Suite<T, J>::runTests(Array<string> testsToRun)
{
    cout << +RED "\nStarting test suite " << suiteName + RESET << endl;
    for (int i = 0; i < testsToRun.getLength(); i++)
    {

        if (*testsToRun[i] == "==")
            equalsTest();
        else if (*testsToRun[i] == "TC")
        {
            textCompare();
        }
        else
        {
            cout << "Illegitimate string given" << endl;
        }
    }
}

template <class T, class J>
Suite<T, J>::~Suite()
{
    delete testObj;
    delete correctObj;
}
// prints the states upon deletion
// requires that T and J have to_String() overloaded
template <class T, class J>
void Suite<T, J>::textCompare()
{
    textCompare(*testObj, *correctObj);
}

template <class T, class J>
template <class X, class Y>
void Suite<T, J>::textCompare(X &lhs, Y &rhs)
{
    cout << "\nRunning text compare" << endl;
    string tstString = to_string(lhs);
    string corString = to_string(rhs);
    string output = "";
    bool testPassed = true;
    int index = 0;

    while (index < tstString.length() && index < corString.length())
    {
        if (tstString[index] == corString[index])
            output += printGreen(index, tstString, corString);
        else
        {
            output += printRed(index, tstString, corString);

            if (testPassed)
            {
                fails++;
                testPassed = false;
            }
        }
    }

    if (tstString.length() != output.length())
    {
        output += YELLOW + tstString.substr(index, tstString.length() - index) + RESET;
    }

    if (testPassed)
        passes++;

    cout << "The output was " << output << "\nThe output should be " << GREEN << corString << RESET << endl;
    cout << "Text compare finished\n"
         << endl;
}

template <class T, class J>
void Suite<T, J>::equalsTest()
{
    equalsTest(*testObj, *correctObj);
}
template <class T, class J>
template <class X, class Y>
void Suite<T, J>::equalsTest(X &lhs, Y &rhs) // makes use of a copy constuctor
{
    cout << "\nStarting equals test" << endl;
    if (lhs == rhs)
    {
        passes++;
        cout << GREEN << "Items are equal" << RESET << endl;
    }
    else
    {
        fails++;
        cout << RED << "Items are not equal" << RESET << endl;
    }

    cout << "ending equals test\n"
         << endl;
}
template <class T, class J>
Suite<T, J> &Suite<T, J>::operator=(Suite<T, J> &copy)
{

    if (this == &copy)
    {
        return *this;
    }
    delete testObj;
    delete correctObj;

    testObj = new T(*copy.testObj);
    correctObj = new J(*copy.correctObj);

    passes = copy.passes;
    fails = copy.fails;
    suiteName = copy.suiteName;

    return *this;
}

template <class T, class J>
string Suite<T, J>::printGreen(int &index, string tstString, string corString)
{
    string output = GREEN;
    while (index < tstString.length() && index < corString.length() && tstString[index] == corString[index])
    {
        output += tstString[index++];
    }

    output += RESET;
    return output;
}
template <class T, class J>
string Suite<T, J>::printRed(int &index, string tstString, string corString)
{
    string output = RED;
    while (index < tstString.length() && index < corString.length() && tstString[index] != corString[index])
    {
        output += tstString[index++];
    }

    output += RESET;
    return output;
}
template <class T, class J>
void Suite<T, J>::setTest(T *testObj)
{

    delete testObj;
    testObj = new T(*testObj);
    // makes a copy
}
template <class T, class J>
void Suite<T, J>::setCorrect(J *corrObj)
{
    delete correctObj;
    correctObj = new J(*correctObj);
}
