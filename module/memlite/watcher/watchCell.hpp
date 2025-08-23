/// @file
#pragma once

#include "memlite/watcher/bindTag.hpp"

namespace by {
    /// @ingroup memlite
    /// @brief Memory watch cell for tracking instance states
    /// @details Immortality of WatchCell instance.
    /// WatchCell is managed by Chunks and Chunks set used instances to some
    /// index for chaining linked list. in order to hand over quickly to
    /// caller when it requires Chunks to alloc new an instance.
    ///
    /// because used up element will be set to integer, which is for index,
    /// if WatchCell has no nidx in front of its member, its blk member-variable
    /// definitely would be polluted and accessing it causes Undefined Behavior.
    ///
    /// and we need the blk that can be used after it freed, this is why we
    /// put an additional value, `next`. see @ref bindTag for more info.
    struct _nout watchCell {
        nidx next;
        bindTag blk;
    };
}
