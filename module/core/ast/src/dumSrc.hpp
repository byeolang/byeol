/// @file
#pragma once

#include "core/ast/src/src.hpp"

namespace by {
    /** @ingroup core
     *  @brief Dummy source location implementation
     *  @details Placeholder source location used when no actual source information is available.
     */
    class _nout dumSrc: public src {
        BY(CLASS(dumSrc, src))

    private:
        dumSrc();

    public:
        void setFile(const srcFile& new1) override;
        void setPos(const point& new1) override;
        void setName(const std::string& new1) override;

        static me& singleton();
    };
}
