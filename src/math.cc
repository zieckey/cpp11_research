#include "test_common.h"

#include <algorithm>
#include <cmath>

TEST_UNIT(pow_test)
{
    H_TEST_ASSERT(std::pow(5, 2) == 25); //5��2����
    H_TEST_ASSERT(std::pow(5, 3) == 125); //5��3����
}


TEST_UNIT(log_test)
{
    //��Ȼ�����ĵ���e����һ����Ҫ���޸�����.���Ƕ��壺��x��������ʱ��lim(1+1/x)^x=e. 
    //e��һ�����޲�ѭ��С������ֵԼ����2.718281828
    //Computes the the natural (base e) logarithm of arg.
    double f1 = std::log(2);
    H_TEST_ASSERT(f1 == 0.69314718055994529);

    std::cout << "log(1) = " << std::log(1) << '\n'
        << "base-5 logarithm of 125 = " << std::log(125) / std::log(5) << '\n';
}