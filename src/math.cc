#include "test_common.h"

#include <algorithm>
#include <cmath>

TEST_UNIT(pow_test)
{
    H_TEST_ASSERT(std::pow(5, 2) == 25); //5��2����
    H_TEST_ASSERT(std::pow(5, 3) == 125); //5��3����
}