#pragma once

#include "../common.hpp"
#include "../macro.hpp"
#include <functional>

namespace nm {

    //  end:
    //      end is like a 'defer' keyword in other languages.
    //      in namulang, it registers what to be done when you go out of the current func.
    //      in c++, but, it limited to scope, not a func.
    class _nout end {
        NM(ME(end))

    public:
        end(std::function<void(void)> l);
        ~end();

    private:
        std::function<void(void)> _lambda;
    };

// e.g.
//  NM_END_BLOCK({
//      ....doSomething()
//  });
//      or,
//  NM_END(doSomething());
#define NM_END_BLOCK(blockStmt) auto __defer__##__COUNTER__ = end([&]() blockStmt)
#define NM_END(stmt) auto __defer__##__COUNTER__ = end([&]() { stmt; })
}
