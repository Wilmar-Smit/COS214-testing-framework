#include "testing.h"
// ironically used to test the testing framework
int main()
{
    cout << "Array testing \n"
         << endl;
    Array<int> arr1(5);
    for (int i = 0; i < 5; i++)
    {
        arr1[i] = i;
    }
    cout << to_string(arr1) << endl;

    Array<int> arr2(10);

    arr2 = arr1;
   cout << to_string(arr2)<<endl;



    string *test1 = new string("107100");
    string *test2 = new string("109");


    Array<string> arrStr(1);
    arrStr.insertNewItem("TC");
    arrStr.insertNewItem("==");

    // testing basic strings
    Suite<string, string> suiteString(arrStr, test1, test2, "basic compare");


    // testing the array class
    Suite<Array<int>,Array<int>> suiteArr(arrStr,&arr1,&arr2,"Testing arrays in the suite");
    

    delete test1;
    delete test2;
    return 0;
}
