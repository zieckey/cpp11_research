#include "test_common.h"

#include <algorithm>
#include <cmath>

TEST_UNIT(pow_test)
{
    H_TEST_ASSERT(std::pow(5, 2) == 25); //5的2次幂
    H_TEST_ASSERT(std::pow(5, 3) == 125); //5的3次幂
}