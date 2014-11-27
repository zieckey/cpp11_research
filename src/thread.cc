#include "test_common.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

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

TEST_UNIT(thread_test1)
{
    User u;
    u.name = "zieckey";
    u.addr = "beijing";
    std::atomic<int> count;
    std::thread t1(CountInc, "thread1", &count);
    std::thread t2(&CountInc, "thread2", &count);
    std::thread t3(std::bind(&User::Print, &u, "user", &count));
    t1.join();
    t2.join();
    t3.join();
    H_TEST_ASSERT(count == 3);
}


namespace
{

    std::mutex m;
    std::condition_variable event;
    std::string data;
    bool ready = false;
    bool processed = false;

    void worker_thread()
    {
        // Wait until main() sends data
        std::unique_lock<std::mutex> lk(m);
        event.wait(lk, []{return ready; });

        // after the wait, we own the lock.
        std::cout << "Worker thread is processing data\n";
        data += " after processing";

        // Send data back to main()
        processed = true;
        std::cout << "Worker thread signals data processing completed\n";

        // Manual unlocking is done before notifying, to avoid waking up
        // the waiting thread only to block again (see notify_one for details)
        lk.unlock();
        event.notify_one();
    }

}

TEST_UNIT(thread_test2)
{
    std::thread worker(worker_thread);

    data = "Example data";
    // send data to the worker thread
    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    event.notify_one();

    // wait for the worker
    {
        std::unique_lock<std::mutex> lk(m);
        event.wait(lk, []{return processed; });
    }
    std::cout << "Back in main(), data = " << data << '\n';

    worker.join();
}
