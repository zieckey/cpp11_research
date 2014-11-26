#include "test_common.h"

#include <iostream>
#include <thread>
#include <atomic>

static void func(const std::string& name, std::atomic<int>* count)
{
    count->fetch_add(1);
    std::cout << "Thread id=" << std::this_thread::get_id() << " name=" << name << std::endl;
    std::flush(std::cout);
}

TEST_UNIT(thread_test)
{
    std::atomic<int> count;
    std::thread t1(func, "thread1", &count);
    std::thread t2(func, "thread2", &count);
    t1.join();
    t2.join();
    H_TEST_ASSERT(count == 2);
}

