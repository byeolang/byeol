#include "../../common/dep.hpp"
#include "../../namuSyntaxTest.hpp"
#include <functional>
#include <chrono>
#include <cstdint>

using namespace namu;
using namespace std;

struct speedTest : namuSyntaxTest {};

namespace  {
    void benchMark(string name, int cnt, function<void(void)> func) {
        logger::get().setEnable(false);

        auto start = chrono::steady_clock::now();
        for(int n=0; n < cnt; n++)
            func();
        auto end = chrono::steady_clock::now();
        auto totalElapsed = end - start;

        logger::get().setEnable(true);
        NAMU_I("[%s]: it took total %d ms.", name.c_str(), totalElapsed / chrono::milliseconds(1));
        logger::get().setEnable(false);
    }

    struct myObj : public obj {
        NAMU(CLASS(myObj, obj))
    };

    struct dummy {
        dummy() {}
        dummy(const std::string& newName): name(newName) {}

        std::string name;
    };
}

TEST_F(speedTest, benchmarkArr) {
    int clc = 0;
    myObj obj1;
    benchMark("create narr 10000 times", 10000, [&clc]() {
        narr ar;
        clc += ((int64_t) &ar) % 2;
    });
    benchMark("create arr 10000 times", 10000, [&clc]() {
        arr ar;
        clc += ((int64_t) &ar) % 2;
    });

    benchMark("create vector 10000 times", 10000, [&clc]() {
        vector<node> ar;
        clc += ((int64_t) &ar) % 2;
    });

    benchMark("create obj 10000 times", 10000, [&clc]() {
        myObj ar;
        clc += ((int64_t) &ar) % 2;
    });

    benchMark("create nchain 10000 times", 10000, [&clc]() {
        nchain ar;
        clc += ((int64_t) &ar) % 2;
    });

    benchMark("add narr 10000 times", 10000, [&]() {
        narr ar;
        ar.add(obj1);
        clc += ((int64_t) &ar) % 2;
    });
    benchMark("add arr 10000 times", 10000, [&]() {
        arr ar;
        ar.add(obj1);
        clc += ((int64_t) &ar) % 2;
    });

    benchMark("add vector 10000 times", 10000, [&]() {
        vector<str> ar;
        ar.push_back(str(obj1));
        clc += ((int64_t) &ar) % 2;
    });

    benchMark("add nchain 10000 times", 10000, [&]() {
        nchain ar;
        ar.add("", obj1);
        clc += ((int64_t) &ar) % 2;
    });
}

TEST_F(speedTest, benchmarkNodeCreation) {
    std::uintptr_t i = 0;
    benchMark("create 10000 dummy object", 10000, [&]() {
        dummy dum;
        i += reinterpret_cast<std::uintptr_t>(&dum);
    });
    benchMark("create 10000 dummy object with string", 10000, [&]() {
        dummy dum("wow");
        i += reinterpret_cast<std::uintptr_t>(&dum);
    });
    benchMark("create 10000 myObj object", 10000, [&]() {
        myObj dum;
        i += dum.getId().serial + 1;
    });
    benchMark("create 10000 nInt object", 10000, [&]() {
        nInt dum;
        i += dum.getId().serial + 1;
    });
    ASSERT_TRUE(i >= 40000);
}

TEST_F(speedTest, benchmarkStringCreation) {
    std::uintptr_t i = 0;
    benchMark("create 10000 std::string", 10000, [&]() {
        std::string dum("hello world");
        i += reinterpret_cast<std::uintptr_t>(&dum);
    });
    benchMark("create 10000 nStr", 10000, [&]() {
        nStr dum("hello world");
        i += reinterpret_cast<std::uintptr_t>(&dum);
    });
    ASSERT_TRUE(i >= 20000);
}

TEST_F(speedTest, benchmarkSumOfSequence) {
    NAMU_I("sum 10000 of integer:");

    nint sum = 0;
    auto start = chrono::steady_clock::now();
    for(int n=0; n < 10000;n++)
        sum += n;
    auto end = chrono::steady_clock::now();

    make().parse(R"SRC(
        main() int
            sum := 0
            for n in 1..10000
                sum = sum + n
            return sum
    )SRC").shouldVerified(true);

    auto start2 = chrono::steady_clock::now();
    str res = run();
    auto end2 = chrono::steady_clock::now();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), sum);
    logger::get().setEnable(true);
    NAMU_I("native time: %d", (end - start) / chrono::milliseconds(1));
    NAMU_I("managed time: %d", (end2 - start2) / chrono::milliseconds(1));
    logger::get().setEnable(false);
}
