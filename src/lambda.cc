#include "test_common.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

TEST_UNIT(lambda_test1)
{
    std::vector<int> c{ 1, 2, 3, 4, 5, 6, 7 };
    int x = 5;
    c.erase(std::remove_if(c.begin(), c.end(), [x](int n) { return n < x; }), c.end());

    std::cout << "c: ";
    for (auto i : c) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    // the type of a closure cannot be named, but can be inferred with auto
    auto func1 = [](int i) { return i + 4; };
    std::cout << "func1: " << func1(6) << '\n';

    // like all callable objects, closures can be captured in std::function
    // (this may incur unnecessary overhead)
    std::function<int(int)> func2 = [](int i) { return i + 4; };
    std::cout << "func2: " << func2(6) << '\n';
}



using namespace std;

TEST_UNIT(lambda_test2)
{

    // Create a vector object that contains 10 elements.
    vector<int> v;
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }

    // Count the number of even numbers in the vector by 
    // using the for_each function and a lambda.   这里相当于创建了一个函数对象，构造函数的参数为（&evenCount）
    int evenCount = 0;
    for_each(v.begin(), v.end(), [&evenCount](int n) {
        cout << n;

        if (n % 2 == 0) {
            cout << " is even " << endl;
            ++evenCount;
        }
        else {
            cout << " is odd " << endl;
        }
    });

    // Print the count of even numbers to the console.
    cout << "There are " << evenCount
        << " even numbers in the vector." << endl;
}


TEST_UNIT(lambda_test3)
{
    int x = 10;
    int y = 3;
    int z = 0;

    //因为是以值传递的方式访问x,y所以x,y的值并没有发生改变
    z = [=]()mutable throw() -> int { int n = x + y; x = y; y = n; return n; }();
    cout << "z:" << z << "\tx:" << x << "\t" << "y:" << y << endl;
    H_TEST_ASSERT(x == 10);
    H_TEST_ASSERT(y == 3);
    H_TEST_ASSERT(z == 13);

    //因为是以引用传递的方式访问x,y所以x,y的值已经发生改变
    z = [&]()mutable throw() -> int { int n = x + y; x = y; y = n; return n; }();
    cout << "z:" << z << "\tx:" << x << "\t" << "y:" << y << endl;
    H_TEST_ASSERT(y == 13);
    H_TEST_ASSERT(x == 3);
    H_TEST_ASSERT(z == 13);
}