/// @file
#pragma once

#include "clog/common.hpp"

namespace by {

    typedef std::map<std::string, nbool> enables;

    /// @ingroup clog
    /// @brief Temporary scope-based enable/disable zone for logging streams
    /// @details Provides RAII-style temporary modification of logging stream states.
    /// Automatically restores previous states when the zone goes out of scope.
    class _nout enablesZone {
        BY(ME(enablesZone))

    public:
        enablesZone();
        enablesZone(nbool newVal);
        ~enablesZone();

    public:
        const enables& getPrev() const;
        me& setPrev();
        me& rel();

        me& setEnable(nbool newVal);
        me& setEnable(const std::string& streamName, nbool newVal);

    private:
        enables _enables;
    };
}
