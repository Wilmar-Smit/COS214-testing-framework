#include "testing.h"
// ironically used to test the testing framework
int main()
{
    string *test1 = new string("107100");
    string *test2 = new string("109");

    string instruction = "TC";
    Array<string> arrStr(1);
    arrStr.insertNewItem(instruction);
    Suite<string, string> suiteString(arrStr, test1, test2, "basic compare");

    delete test1;
    delete test2;
    return 0;
}
