#pragma once

#include "indep/common.hpp"
#include "indep/macro.hpp"

namespace by {

    //  end:
    //      end is like a 'defer' keyword in other languages.
    //      in byeolang, it registers what to be done when you go out of the current func.
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
//  BY_END_BLOCK({
//      ....doSomething()
//  });
//      or,
//  BY_END(doSomething());
#define BY_END_BLOCK(blockStmt) auto __defer__##__COUNTER__ = end([&]() blockStmt)
#define BY_END(stmt) auto __defer__##__COUNTER__ = end([&]() { stmt; })
}
