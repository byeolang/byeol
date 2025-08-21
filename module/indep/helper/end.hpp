/// @file
#pragma once

#include "indep/common.hpp"
#include "indep/macro.hpp"

namespace by {

    /// @ingroup indep
    /// @brief Deferred execution utility like defer keyword in other languages
    /// @details End is like a 'defer' keyword in other languages.
    /// In byeolang, it registers what to be done when you go out of the current func.
    /// In C++, but, it limited to scope, not a func.
    class _nout end {
        BY(ME(end))

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
