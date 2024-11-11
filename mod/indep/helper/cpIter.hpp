#pragma once

#include "../common.hpp"
#include "../macro.hpp"

namespace nm {
    /// iterator for codepoint of string.
    class cpIter {
        NM(ME(cpIter))

    public:
        cpIter(const nchar* from);
        cpIter(const std::string& from);

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
        ncnt next(ncnt step);
        std::string get() const;
        ncnt remainLen() const;

    private:
        const nchar* _nextCodepoint(const nchar* from) const;
        ncnt _skipBytes(nchar ch) const;

    private:
        const nchar* _e;
    };
}
