/// @file
#pragma once

#include "clog/common/dep.hpp"

namespace by {
    /**
     * @ingroup clog
     * @brief Helper class to finalize scope logging with elapsed time.
     * @details Decrements graph level and returns elapsed milliseconds.
     * @param start The start time point of the scope.
     */
    class _nout scopeLog : public end {
        BY(ME(scopeLog, end))

    public:
        scopeLog(const std::function<void(nllong)>& l);

    private:
        static nllong _measureNow();

    private:
        nllong _prev;
    };
} // namespace by
