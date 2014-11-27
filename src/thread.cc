#include "test_common.h"

#include <iostream>
#include <thread>
#include <atomic>

static void CountInc(const std::string& name, std::atomic<int>* count)
{
    count->fetch_add(1);
    std::cout << "Thread id=" << std::this_thread::get_id() << " name=" << name << std::endl;
    std::flush(std::cout);
}

namespace
{
    struct User
    {
        std::string name;
        std::string addr;

        void Print(const std::string& message, std::atomic<int>* count)
        {
            count->fetch_add(1);
            std::cout << "Thread id=" << std::this_thread::get_id() << " message=" << message << " name=" << name << " addr=" << addr << std::endl;
        }
    };
}

TEST_UNIT(thread_test)
{
    User u;
    u.name = "zieckey";
    u.addr = "beijing";
    std::atomic<int> count(0);
    std::thread t1(CountInc, "thread1", &count);
    std::thread t2(&CountInc, "thread2", &count);
    std::thread t3(std::bind(&User::Print, &u, "user", &count));
    t1.join();
    t2.join();
    t3.join();
    H_TEST_ASSERT(count == 3);
}

