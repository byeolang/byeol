#pragma once

#include "../common.hpp"
#include "../macro.hpp"

namespace nm {
    /// iterator for codepoint of string.
    class _nout cpIter {
        NM(ME(cpIter))

    public:
        cpIter(const nchar* begin, const nchar* from);
        cpIter(const std::string& from, nbool isReversed = false);

    public:
        me operator+(ncnt step);
        me& operator++();
        me operator++(int);
        me& operator+=(ncnt step);
        std::string operator*() const;
        explicit operator nbool() const;
        nbool operator==(const me& rhs) const;

    public:
        nbool isEnd() const;

        void rel();

        /// Moves the iterator in the direction you specified when you created it.
        /// if you created a `reverse iterator`, this func will move it by steps in the reverse
        /// direction.
        /// @return stepped count.
        ncnt next(ncnt step);

        /// moves `iterator` to  unconditionally forward by the given step.
        /// note that creating a reverse iterator doesn't affect this function.
        /// so it'll also move to forward direction.
        /// @return stepped count.
        ncnt stepForward(ncnt step);
        ncnt stepBackward(ncnt step);

        std::string get() const;
        ncnt remainLen() const;

    private:
        ncnt _step(std::function<const nchar*()> closure, ncnt step);
        const nchar* _nextCodepoint(const nchar* from) const;
        const nchar* _prevCodepoint(const nchar* e) const;
        ncnt _skipBytes(nchar ch) const;

    private:
        const nchar* _begin;
        const nchar* _end;
        nbool _isReverse;
    };
}
