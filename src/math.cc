#include "test_common.h"

#include <algorithm>
#include <cmath>

TEST_UNIT(pow_test)
{
    H_TEST_ASSERT(std::pow(5, 2) == 25); //5的2次幂
    H_TEST_ASSERT(std::pow(5, 3) == 125); //5的3次幂
}


TEST_UNIT(log_test)
{
    //自然对数的底数e是由一个重要极限给出的.我们定义：当x趋于无限时，lim(1+1/x)^x=e. 
    //e是一个无限不循环小数，其值约等于2.718281828
    //Computes the the natural (base e) logarithm of arg.
    double f1 = std::log(2);
    H_TEST_ASSERT(f1 == 0.69314718055994529);

    std::cout << "log(1) = " << std::log(1) << '\n'
        << "base-5 logarithm of 125 = " << std::log(125) / std::log(5) << '\n';
}