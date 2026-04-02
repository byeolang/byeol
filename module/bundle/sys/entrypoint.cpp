#include <iostream>

#include "bundle/sys/common.hpp"
using namespace by;

struct con {
    void say() { std::cout << "hello world!\n"; }

    int add(int a, int b) { return a + b; }

    const std::string& print(const std::string& msg) {
        BY_W("=========================");
        for(int n = 0; n < 10; n++) {
            const nchar* c = msg.c_str();
            BY_W("[%d]=%d", n, (int) c[n]);
        }
        BY_W("=========================");
        std::cout << msg;
        return msg;
    }

    std::string input() {
        std::string ret;
        std::cin >> ret;
        return ret;
    }
};

extern "C" _nout void byeol_bridge_cpp_entrypoint(bicontainable* tray) {
    tray->add("con", tbridger<con>::ctor()
            .ctor<con>()
            .func("say", &con::say)
            .func("add", &con::add)
            .func("print", &con::print)
            .func("input", &con::input)
            .make(new con()));
}
