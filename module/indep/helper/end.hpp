/// @file
#pragma once

#include "indep/common.hpp"
#include "indep/macro.hpp"

namespace by {

    /** @ingroup indep
     *  @brief Deferred execution utility similar to defer keyword in other languages
     *  @details Delays code execution until the end of the current scope. Similar to
     *  the `defer` keyword found in languages like Go or Swift.
     *
     *  In byeolang, end registers code to be executed when exiting the current function.
     *  In C++, this is implemented using RAII and is limited to scope boundaries rather
     *  than function boundaries.
     *
     *  @section usage Usage
     *  Use the provided macros for convenient deferred execution:
     *  @code
     *      void processFile() {
     *          FILE* f = fopen("data.txt", "r");
     *          BY_END(fclose(f)); // Automatically closes file when leaving scope
     *
     *          // Or use block form for multiple statements:
     *          BY_END_BLOCK({
     *              cleanupResource();
     *              logCompletion();
     *          });
     *
     *          // ... do work with file ...
     *      } // fclose(f) executes here automatically
     *  @endcode
     */
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
#define BY_END_BLOCK_2(__class, blockStmt) auto BY_CONCAT(__defer__, __COUNTER__) = __class([&]() blockStmt)
#define BY_END_BLOCK_1(blockStmt) BY_END_BLOCK_2(end, blockStmt)
#define BY_END_BLOCK(...) BY_OVERLOAD(BY_END_BLOCK, __VA_ARGS__)

#define BY_END_2(__class, stmt) auto BY_CONCAT(__defer__, __COUNTER__) = __class([&]() { stmt; })
#define BY_END_1(stmt) BY_END_2(end, (stmt))
#define BY_END(...) BY_OVERLOAD(BY_END, __VA_ARGS__)
}
