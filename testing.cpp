#include "testing.h"

// ############################ testing code ############################
template <class T, class J>
testing<T, J>::testing(T testObject, J correctObject)
{
    this->passes = 0;
    this->fails = 0;
    this->testObject = new T(testObject);
    this->correctObject = new J(correctObject);
    testSuites = new Array<Suite<T, J>>(0);
}

template <class T, class j>
testing<T, j>::~testing()
{
    delete testObject;
    delete correctObject;
    delete testSuites;
}

template <class T, class J>
T *testing<T, J>::getTestObj()
{
    return testObject;
}

template <class T, class J>
J *testing<T, J>::getCorrectObj()
{
    return correctObject;
}
template <class T, class J>
void testing<T, J>::createTestSuite(Array<string> testsToRun, string suiteName)
{
   Suite<T, J> nSuite(testsToRun, testObject, correctObject, suiteName);
    testSuites->insertNewItem(nSuite);
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
        else if ((*testsToRun[i]).substr(0, 3) == "TC=")
        {
            string testStr = testsToRun[i]->substr(4, testsToRun[i]->length() - 1);
            testCompare(testStr);
        }
    }
}

template <class T, class J>
Suite<T, J>::~Suite()
{
    // dont delete testobj and correctObj

    delete testObj;
    delete correctObj;

}
// prints the states upon deletion
// requires that T and J have to_String() overloaded
template <class T, class J>
void Suite<T, J>::textCompare()
{
    cout << "\nRunning text compare" << endl;
    string tstString = to_string(*testObj);
    string corString = to_string(*correctObj);
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
void Suite<T, J>::testCompare(string testAgainst)
{
}

template <class T, class J>
void Suite<T, J>::equalsTest()
{
    cout << "\nStarting equals test" << endl;
    if (*testObj == *correctObj)
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
template <class X, class Y>
void Suite<T, J>::equalsTest(X lhs, Y rhs) // makes use of a copy constuctor
{
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
