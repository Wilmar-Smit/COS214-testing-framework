#include "testing.h"
// ironically used to test the testing framework
int main()
{
    Array<string> arrStr(1);
    arrStr.insert("TC");
    arrStr.insert("==");

    string test1 = ("107100");
    string test2 = ("109");

    // testing basic strings
    Suite<string, string> suiteString(arrStr, test1, test2, "basic compare");

    // testing the array class
    cout << "Array testing \n"
         << endl;
    Array<int> arr1(5);

    for (int i = 0; i < 5; i++)
    {
        arr1.insertNewItem(i);
    }

    cout << to_string(arr1) << endl;
    Array<int> arr2(10);

    Suite<Array<int>, Array<int>> suiteArrBefore(arrStr, arr1, arr2, "Testing arrays in the suite");

    arr2 = arr1;

    cout << to_string(arr2) << endl;
    Suite<Array<int>, Array<int>> suiteArrAfter(arrStr, arr1, arr2, "Testing arrays in the Testing class");

    Testing<string, string> *Ts = new Testing<string, string>(test1, test2);
    Ts->createTestSuite(arrStr, "basic compare in TS object");

    Testing<Array<int>, Array<int>> *TsArr = new Testing<Array<int>, Array<int>>(arr2, arr1);
    TsArr->createTestSuite(arrStr, "Arr compare in TSarr object");

    delete Ts;
    delete TsArr;
    // add ==, = and copy cons for the suite class to make use of testing class.
    return 0;
}
