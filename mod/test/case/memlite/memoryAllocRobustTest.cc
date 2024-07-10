#include "../../namuTest.hpp"
#include <ctime>

using namespace nm;

struct memoryAllocRobustTest : public namuTest {};

namespace {
    struct A : public instance {
        const type& getType() const {
            return ttype<A>::get();
        }

        clonable* clone() const {
            return new A();
        }

        int age;
    };


    struct B : public A {
        const type& getType() const {
            return ttype<B>::get();
        }

        clonable* clone() const {
            return new B();
        }

        float grade;
    };

    struct pinstance { id _id; };
    struct pnode : public pinstance {};
    struct pobject : public pnode {};

    struct PA : public pobject {
        const type& getType() const {
            return ttype<PA>::get();
        }

        int age;
    };

    struct PB : public PA {
        const type& getType() const {
            return ttype<PB>::get();
        }

        float grade;
    };

    time_t run1(int& crc, int n)
    {
        PA* parr[100000] = {0, };
        time_t start = clock();
        crc = 0;
        for(int i=0; i < n ;i++) {
            parr[i] = new PB();
            crc += *(int*) parr[i];
        }
        for(int i=0; i < n ;i++)
            delete parr[i];
        return clock() - start;
    }

    time_t run2(int& crc, int n)
    {
        crc = 0;
        A* arr[100000] = {0, };
        time_t start = clock();
        for(int i=0; i < n ;i++) {
            arr[i] = new B();
            crc += *(int*) arr[i];
        }
        for(int i=0; i < n ;i++)
            delete arr[i];
        return clock() - start;
    }

}

#define SPRINT(n) \
    NM_W("%d times new/delete : %f ms elapsed. crc=%d", n, ((float) run1(crc, (n))) / CLOCKS_PER_SEC*1000.0f, crc); \
    NM_W("%d times mempool    : %f ms elapsed. crc=%d", n, ((float) run2(crc, (n))) / CLOCKS_PER_SEC*1000.0f, crc);

TEST_F(memoryAllocRobustTest, sprint10) {
    int crc = 0;
    SPRINT(10)
}

TEST_F(memoryAllocRobustTest, sprint100) {
    int crc = 0;
    SPRINT(100)
}

TEST_F(memoryAllocRobustTest, sprint1000) {
    int crc = 0;
    SPRINT(1000)
}

TEST_F(memoryAllocRobustTest, sprint10000) {
    int crc = 0;
    SPRINT(10000)
}

TEST_F(memoryAllocRobustTest, sprint50000) {
    int crc = 0;
    SPRINT(50000)
}

TEST_F(memoryAllocRobustTest, sprint100000) {
    int crc = 0;
    SPRINT(100000)
}
