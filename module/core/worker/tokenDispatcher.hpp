/// @file
#pragma once

#include <list>

#include "core/common.hpp"

namespace by {
    /** @ingroup core
     *  @brief Token queue management for parser
     *  @details Manages a queue of tokens for lookahead and token reordering during parsing.
     *  Provides FIFO token dispatch with front insertion capability.
     */
    class _nout tokenDispatcher {
    public:
        void add(nint newToken);
        void addFront(nint newToken);
        /**
         * @brief Removes a token from the front of the queue.
         * @param output Reference to store the popped token ID.
         * @return true if a token was successfully popped, false if the queue was empty.
         */
        nbool pop(nint& output);

        ncnt len() const;

        void rel();

    private:
        std::list<nint> _toks;
    };
}
