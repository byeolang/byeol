/// @file
#pragma once

#include <list>

#include "core/common.hpp"

namespace by {
    /** @ingroup core
     *  @brief Token queue management for parser
     *  @details Manages a queue of tokens for lookahead and token reordering during parsing.
     *  Provides FIFO token dispatch with front insertion capability.
     *
     *  @section you must return token in your rule after using dispatcher
     *           if you don't return a token in flex file, dispatcher won't work.
     *           it's by implementation of flex. for example,
     *           @code
     *              yourRule {
     *                  PS.getDispatcher().add('+');
     *                  // you didn't return here because you want to feed '+' back.
     *                  // however, by implementation of flex, there is no chance
     *                  // to trigger tokenDispatcher.
     *              }
     *              yourSecondRule {
     *                  PS.getDispatcher().add('+');
     *                  return STRVAL; // you returns some token. now next token will be '+'.
     *              }
     *          @endcode
     *
     *
     *  @section it only takes token, not character
     *           you may confuse that this class is just like `unput()`, but it's totally different.
     *           unput() affects stream itself. however, tokenDispatchers skips lexer and returns
     *           determined token forcely, not a single character.
     *           for example,
     *           @code
     *              auto& disp = PS.getDispatcher();
     *              disp.add('n');
     *              disp.add('a');
     *              disp.add('m');
     *              disp.add('e');
     *           @endcode
     *           you may think that above codes exactly same to `unput('e'); unput('m'); ....`.
     *           but that's not true. if you use unput(), lexer will judge the token as STRVAL
     *           which contains string value, "name".
     *           however, above codes, returns 'n' token individually to parser which makes it
     *           fail to continue.
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
