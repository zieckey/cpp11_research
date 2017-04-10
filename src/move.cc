#include "test_common.h"

#include <iostream>
#include <functional>

class Moveable{
public:
    Moveable() : i(new int(3)) {
        std::cout << "Moveable::Moveable() 构造函数 : ptr(i)=" << i << std::endl;
    }
    ~Moveable() { 
        std::cout << "Moveable::~Moveable() 析构函数 ptr(i)=" << i << std::endl;
        if (i) {
            delete i;
            i = nullptr;
        }
    }
    Moveable(const Moveable & m) : i(new int(*m.i)) {
        std::cout << "Moveable::Moveable(const Moveable &) 拷贝构造函数 ptr(i)=" << i << std::endl;
    }
    Moveable(Moveable && m) {
        this->i = m.i;
        m.i = nullptr;
        std::cout << "Moveable::Moveable(Moveable &&) 移动构造函数 ptr(i)=" << i << std::endl;
    }

    void Set(int x) {
        *i = x;
    }

    const int* Get() const {
        return i;
    }
private:
    int* i;
};

void stdmove_test1() {
    Moveable a;
    a.Set(5);
    const int* ptr = a.Get();

    Moveable b(a); // 这里会为b.i重新分配内存

    std::cout << "a ptr=" << a.Get() << "\n";
    std::cout << "b ptr=" << b.Get() << "\n";
    H_TEST_ASSERT(a.Get() == ptr);
    H_TEST_ASSERT(a.Get() != nullptr);
    H_TEST_ASSERT(a.Get() != b.Get());
    H_TEST_ASSERT(b.Get() != nullptr);

    /* Output:
    Moveable::Moveable() 构造函数 : ptr(i)=006AC498
    Moveable::Moveable(const Moveable &) 拷贝构造函数 ptr(i)=006AC4D8
    a ptr=006AC498
    b ptr=006AC4D8
    Moveable::~Moveable() 析构函数 ptr(i)=006AC4D8
    Moveable::~Moveable() 析构函数 ptr(i)=006AC498
    */
}


void stdmove_test2() {
    Moveable a;
    a.Set(5);
    const int* ptr = a.Get();

    Moveable b(std::move(a)); // 会调用移动构造函数，因此不会为b.i重新分配内存，而是直接使用a.i指向的内存

    std::cout << "a ptr=" << a.Get() << "\n";
    std::cout << "b ptr=" << b.Get() << "\n";
    H_TEST_ASSERT(a.Get() != ptr);
    H_TEST_ASSERT(a.Get() == nullptr);
    H_TEST_ASSERT(a.Get() != b.Get());
    H_TEST_ASSERT(b.Get() == ptr);
    H_TEST_ASSERT(b.Get() != nullptr);

    /* Output:
    Moveable::Moveable() 构造函数 : ptr(i)=006AC498
    Moveable::Moveable(Moveable &&) 移动构造函数 ptr(i)=006AC498
    a ptr=00000000
    b ptr=006AC498
    Moveable::~Moveable() 析构函数 ptr(i)=006AC498
    Moveable::~Moveable() 析构函数 ptr(i)=00000000
    */
}

void print(Moveable&& h) {
    std::cout << __func__ << " 调用移动语义的参数 ptr(i)=" << h.Get() << std::endl;
}

void print(const Moveable& h) {
    std::cout << __func__ << " 调用const普通语义的参数 ptr(i)=" << h.Get() << std::endl;
}

void stdmove_test3() {
    Moveable m;
    print(m);
    print(std::move(m));
    /*
    Moveable::Moveable() 构造函数 : ptr(i)=00520DD0
    print 调用const普通语义的参数 ptr(i)=00520DD0
    print 调用移动语义的参数 ptr(i)=00520DD0
    Moveable::~Moveable() 析构函数 ptr(i)=00520DD0
    */
}

typedef std::function<void()> Handler;
class Timer {
public:
    Timer(const Handler& h) : fn_(h) {}
    Timer(Handler&& h) : fn_(std::move(h)) {}

    void OnTimer() { fn_(); }
private:
    Handler fn_;
};

void stdmove_test4() {
    // TODO
}



TEST_UNIT(stdmove_test) {
    stdmove_test1();
    std::cout << std::endl;
    stdmove_test2();
    std::cout << std::endl;
    stdmove_test3();
    std::cout << std::endl;
    stdmove_test4();
}