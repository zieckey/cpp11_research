#include "test_common.h"

#include <algorithm>
#include <cmath>

namespace
{
    using namespace std;

    //bloom filter parameter
    struct bfparam
    {
        double e;//false positive probability 预设的可忍受的最高错误率
        size_t n;//estimated element insertion count 预设的最大存储item个数


        size_t m;//the total number of bits for this bloom filter 由e、n计算出来的最小存储空间(bits)
        size_t k;//the number of hash values used to set and test bits 每个item所需的计算次数和bit位

        // p is the fill ratio of the filter partitions. It's mainly used to calculate m at the start.
        // p is not checked when new items are added. So if the fill ratio goes above p, the likelihood
        // of false positives (error rate) will increase.
        //
        // By default we use the fill ratio of p = 0.5
        float p;

    };

    void find_optimal_parameters(size_t predicted_element_count_, double desired_false_positive_probability_)
    {
        /*
        Note:
        The following will attempt to find the number of hash functions
        and minimum amount of storage bits required to construct a bloom
        filter consistent with the user defined false positive probability
        and estimated element insertion count.
        */
        double min_m = std::numeric_limits<double>::infinity();

        double min_k = 0.0;
        double curr_m = 0.0;
        for (double k = 0.0; k < 1000.0; ++k)
        {
            if ((curr_m = ((-k * predicted_element_count_) / std::log(1.0 - std::pow(desired_false_positive_probability_, 1.0 / k)))) < min_m)
            {
                min_m = curr_m;
                min_k = k;
            }
        }

        const size_t bits_per_char = 8;
        size_t salt_count_ = static_cast<std::size_t>(min_k);
        size_t table_size_ = static_cast<std::size_t>(min_m);
        table_size_ += (((table_size_ % bits_per_char) != 0) ? (bits_per_char - (table_size_ % bits_per_char)) : 0);


        double e = desired_false_positive_probability_;
        double p = 0.5;
        double n = table_size_;

        //see https://github.com/dataence/bloom/blob/master/bloom.go
        size_t another_m1 = (ceil(double(n) / ((std::log(p) * std::log(1 - p)) / std::abs(std::log(e))))) / bits_per_char;
        size_t another_k1 = ceil(std::log(1 / e) / std::log(2));

        //see https://github.com/willf/bloom/blob/master/bloom.go
        size_t another_m2 = size_t(-1 * double(n) * std::log(e) / std::pow(std::log(2), 2)) / 8;
        size_t another_k2 = size_t(ceil(std::log(2) * double(another_m2) / double(n)));

        //see http://www.sjsjw.com/kf_www/article/33_25142_28870.asp
        //m >= log2(e) * (n * log2(1/E))
        //(4)给定m和n，可以确定最优hash个数，即k = ln2 * (m/n)，此时错误率最小；
        size_t another_m3 = n * std::log2(e) * std::log2(1 / e) / bits_per_char;
        size_t another_k3 = size_t(std::log(2)*(double(another_m3*bits_per_char) / n));

        //see https://github.com/jaybaird/python-bloomfilter/blob/master/pybloom/pybloom.py
        // # given M = num_bits, k = num_slices, P = error_rate, n = capacity
        // #       k = log2(1 / P)
        // # solving for m = bits_per_slice
        // # n ~= M * ((ln(2) ** 2) / abs(ln(P)))
        // # n ~= (k * m) * ((ln(2) ** 2) / abs(ln(P)))
        // # m ~= n * abs(ln(P)) / (k * (ln(2) ** 2))
        // bits_per_slice = int(math.ceil((capacity * abs(math.log(error_rate))) /(num_slices * (math.log(2) ** 2))))
        size_t another_k4 = ceil(std::log2(1 / e));
        size_t another_m4 = size_t(std::ceil((n * abs(std::log(e))) / (another_k4 * (std::pow(std::log(2), 2)))));
        
        // see https://github.com/seomoz/pyreBloom/blob/master/pyreBloom/bloom.c
        // m: ctxt->bits = (uint64_t)(-(log(error) * capacity) / (log(2) * log(2)));
        // k: ctxt->hashes = (uint32_t)(ceil(log(2) * ctxt->bits / capacity));        
        size_t another_m5 = (size_t)(-(log(e) * n) / (log(2) * log(2))) / bits_per_char;
        size_t another_k5 = (size_t)(ceil(log(2) * another_m4 * bits_per_char / n));

        cout << "n=" << predicted_element_count_
            << " e=" << e
            << " k=" << salt_count_
            << " m=" << table_size_
            << " k1=" << another_k1
            << " m1=" << another_m1
            << " k2=" << another_k2
            << " m2=" << another_m2
            //             << " k3=" << another_k3
            //             << " m3=" << another_m3
            << " k4=" << another_k4
            << " m4=" << another_m4
            << " k5=" << another_k5
            << " m5=" << another_m5
            << endl;

        /*
        n=10 e=0.001 k=10 m=144 k1=10 m1=258 k2=2 m2=258 k4=10 m4=208 k5=9 m5=258
        n=10 e=0.0001 k=13 m=192 k1=14 m1=460 k2=2 m2=460 k4=14 m4=263 k5=8 m5=460
        n=10 e=1e-005 k=17 m=240 k1=17 m1=719 k2=3 m2=718 k4=17 m4=339 k5=8 m5=718
        n=10 e=1e-006 k=20 m=288 k1=20 m1=1035 k2=3 m2=1035 k4=20 m4=415 k5=8 m5=1035
        n=20 e=0.001 k=10 m=288 k1=10 m1=517 k2=2 m2=517 k4=10 m4=415 k5=8 m5=517
        n=20 e=0.0001 k=13 m=384 k1=14 m1=920 k2=2 m2=920 k4=14 m4=526 k5=8 m5=920
        n=20 e=1e-005 k=17 m=480 k1=17 m1=1437 k2=3 m2=1437 k4=17 m4=677 k5=8 m5=1437
        n=20 e=1e-006 k=20 m=576 k1=20 m1=2070 k2=3 m2=2070 k4=20 m4=829 k5=8 m5=2070
        n=30 e=0.001 k=10 m=432 k1=10 m1=776 k2=2 m2=776 k4=10 m4=622 k5=8 m5=776
        n=30 e=0.0001 k=13 m=576 k1=14 m1=1380 k2=2 m2=1380 k4=14 m4=789 k5=8 m5=1380
        n=30 e=1e-005 k=17 m=720 k1=17 m1=2156 k2=3 m2=2156 k4=17 m4=1015 k5=8 m5=2156
        n=30 e=1e-006 k=20 m=864 k1=20 m1=3105 k2=3 m2=3105 k4=20 m4=1243 k5=8 m5=3105
        n=40 e=0.001 k=10 m=576 k1=10 m1=1035 k2=2 m2=1035 k4=10 m4=829 k5=8 m5=1035
        n=40 e=0.0001 k=13 m=768 k1=14 m1=1840 k2=2 m2=1840 k4=14 m4=1052 k5=8 m5=1840
        n=40 e=1e-005 k=17 m=960 k1=17 m1=2875 k2=3 m2=2875 k4=17 m4=1354 k5=8 m5=2875
        n=40 e=1e-006 k=20 m=1152 k1=20 m1=4140 k2=3 m2=4140 k4=20 m4=1657 k5=8 m5=4140
        n=50 e=0.001 k=10 m=720 k1=10 m1=1294 k2=2 m2=1293 k4=10 m4=1036 k5=8 m5=1293
        n=50 e=0.0001 k=13 m=960 k1=14 m1=2300 k2=2 m2=2300 k4=14 m4=1315 k5=8 m5=2300
        n=50 e=1e-005 k=17 m=1200 k1=17 m1=3594 k2=3 m2=3594 k4=17 m4=1692 k5=8 m5=3594
        n=50 e=1e-006 k=20 m=1440 k1=20 m1=5176 k2=3 m2=5175 k4=20 m4=2071 k5=8 m5=5175
        n=60 e=0.001 k=10 m=864 k1=10 m1=1552 k2=2 m2=1552 k4=10 m4=1243 k5=8 m5=1552
        n=60 e=0.0001 k=13 m=1152 k1=14 m1=2760 k2=2 m2=2760 k4=14 m4=1578 k5=8 m5=2760
        n=60 e=1e-005 k=17 m=1440 k1=17 m1=4313 k2=3 m2=4313 k4=17 m4=2030 k5=8 m5=4313
        n=60 e=1e-006 k=20 m=1728 k1=20 m1=6211 k2=3 m2=6211 k4=20 m4=2485 k5=8 m5=6211
        n=70 e=0.001 k=10 m=1008 k1=10 m1=1811 k2=2 m2=1811 k4=10 m4=1450 k5=8 m5=1811
        n=70 e=0.0001 k=13 m=1344 k1=14 m1=3220 k2=2 m2=3220 k4=14 m4=1841 k5=8 m5=3220
        n=70 e=1e-005 k=17 m=1680 k1=17 m1=5032 k2=3 m2=5032 k4=17 m4=2369 k5=8 m5=5032
        n=70 e=1e-006 k=20 m=2016 k1=20 m1=7246 k2=3 m2=7246 k4=20 m4=2899 k5=8 m5=7246
        n=80 e=0.001 k=10 m=1152 k1=10 m1=2070 k2=2 m2=2070 k4=10 m4=1657 k5=8 m5=2070
        n=80 e=0.0001 k=13 m=1536 k1=14 m1=3680 k2=2 m2=3680 k4=14 m4=2104 k5=8 m5=3680
        n=80 e=1e-005 k=17 m=1920 k1=17 m1=5751 k2=3 m2=5751 k4=17 m4=2707 k5=8 m5=5751
        n=80 e=1e-006 k=20 m=2304 k1=20 m1=8281 k2=3 m2=8281 k4=20 m4=3313 k5=8 m5=8281
        n=90 e=0.001 k=10 m=1296 k1=10 m1=2329 k2=2 m2=2329 k4=10 m4=1864 k5=8 m5=2329
        n=90 e=0.0001 k=13 m=1728 k1=14 m1=4140 k2=2 m2=4140 k4=14 m4=2367 k5=8 m5=4140
        n=90 e=1e-005 k=17 m=2160 k1=17 m1=6470 k2=3 m2=6469 k4=17 m4=3045 k5=8 m5=6469
        n=90 e=1e-006 k=20 m=2592 k1=20 m1=9316 k2=3 m2=9316 k4=20 m4=3727 k5=8 m5=9316
        */

        //备注：从上面的试验结果看，<k1,m1>这一对组合是对的。但是目前<k,m>这一对组合是stored里的算法，m较小，因此比较节省内存
        //<k,m>这一对组合看起来也没有问题，只是错误率可能稍高
    }
}


TEST_UNIT(bloomfilter_test)
{
    cout << "find_optimal_parameters:\n";
    size_t element_counts[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90 };
    double error_probalitiy[] = { 0.001, 0.0001, 0.00001, 0.000001 };
    for (int n = 0; n < H_ARRAYSIZE(element_counts); n++)
    {
        for (int j = 0; j < H_ARRAYSIZE(error_probalitiy); j++)
        {
            find_optimal_parameters(element_counts[n], error_probalitiy[j]);
        }
    }
}


