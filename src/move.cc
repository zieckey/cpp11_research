#include "test_common.h"

#include <iostream>

class Moveable{
public:
    Moveable() : i(new int(3)) {
        std::cout << "Moveable::Moveable() ���캯�� : ptr(i)=" << i << std::endl;
    }
    ~Moveable() { 
        std::cout << "Moveable::~Moveable() �������� ptr(i)=" << i << std::endl;
        if (i) {
            delete i;
            i = nullptr;
        }
    }
    Moveable(const Moveable & m) : i(new int(*m.i)) {
        std::cout << "Moveable::Moveable(const Moveable &) �������캯�� ptr(i)=" << i << std::endl;
    }
    Moveable(Moveable && m) {
        this->i = m.i;
        m.i = nullptr;
        std::cout << "Moveable::Moveable(Moveable &&) �ƶ����캯�� ptr(i)=" << i << std::endl;
    }

    void Set(int x) {
        *i = x;
    }

    const int* Get() {
        return i;
    }
private:
    int* i;
};

void stdmove_test1() {
    Moveable a;
    a.Set(5);
    const int* ptr = a.Get();

    Moveable b(a); // �����Ϊb.i���·����ڴ�

    std::cout << "a ptr=" << a.Get() << "\n";
    std::cout << "b ptr=" << b.Get() << "\n";
    H_TEST_ASSERT(a.Get() == ptr);
    H_TEST_ASSERT(a.Get() != nullptr);
    H_TEST_ASSERT(a.Get() != b.Get());
    H_TEST_ASSERT(b.Get() != nullptr);

    /* Output:
    Moveable::Moveable() ���캯�� : ptr(i)=006AC498
    Moveable::Moveable(const Moveable &) �������캯�� ptr(i)=006AC4D8
    a ptr=006AC498
    b ptr=006AC4D8
    Moveable::~Moveable() �������� ptr(i)=006AC4D8
    Moveable::~Moveable() �������� ptr(i)=006AC498
    */
}


void stdmove_test2() {
    Moveable a;
    a.Set(5);
    const int* ptr = a.Get();

    Moveable b(std::move(a)); // ������ƶ����캯������˲���Ϊb.i���·����ڴ棬����ֱ��ʹ��a.iָ����ڴ�

    std::cout << "a ptr=" << a.Get() << "\n";
    std::cout << "b ptr=" << b.Get() << "\n";
    H_TEST_ASSERT(a.Get() != ptr);
    H_TEST_ASSERT(a.Get() == nullptr);
    H_TEST_ASSERT(a.Get() != b.Get());
    H_TEST_ASSERT(b.Get() == ptr);
    H_TEST_ASSERT(b.Get() != nullptr);

    /* Output:
    Moveable::Moveable() ���캯�� : ptr(i)=006AC498
    Moveable::Moveable(Moveable &&) �ƶ����캯�� ptr(i)=006AC498
    a ptr=00000000
    b ptr=006AC498
    Moveable::~Moveable() �������� ptr(i)=006AC498
    Moveable::~Moveable() �������� ptr(i)=00000000
    */
}

TEST_UNIT(stdmove_test) {
    stdmove_test1();
    std::cout << std::endl;
    stdmove_test2();
}