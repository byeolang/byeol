/// @file
#pragma once

#include "indep/common.hpp"
#include "indep/macro.hpp"

namespace by {
    /** @ingroup indep
     *  @brief Codepoint-based string iterator
     *  @details Handles codepoint-based iteration for strings.
     *  Primarily used in @ref nStr for traversing multibyte strings like UTF-8 Unicode.
     *
     *  Like typical iterators, it supports prefix/postfix increment operators,
     *  dereference operator, and bool conversion operator. When creating a cpIter,
     *  you can specify the string to traverse along with the default iteration direction.
     *
     *  @remark Direction and initial position
     *  The direction specified during cpIter creation defines the iterator's default
     *  direction and initial position. If you pass `reverse = true` as argument,
     *  cpIter starts from the end of the string in reverse direction. However, if you
     *  explicitly call stepBackward() or stepForward(), it will move in that direction
     *  regardless of the default direction.
     *
     *
     *  @remark Past-the-end semantics
     *  Like STL, begin points to the first element, but end points to the position
     *  after the last element (past-the-end). Therefore, when traversing in reverse,
     *  you must call next() first.
     *
     *  @section usage Usage
     *  Basic usage example:
     *  @code
     *      std::string src = "abcd🏁efg"; // In UTF-8, 🏁 is represented by 3 characters
     *      cpIter e4(src, true); // Define default iteration as reverse.
     *                            // At this point, it points to past-the-end position
     *      while(*e4 != "") // Currently at end position, so "" is returned
     *          ++e4; // Same as calling e4.next(). Moves in default reverse direction
     *                // But this code never actually executes
     *      ASSERT_EQ(*e4, "");
     *
     *      std::string expects[] = {"g", "f", "e", "🏁", "d", "c", "b", "a"};
     *      for(int n = 0; n < 8; n++) {
     *          // Explicitly specify direction to traverse iterator.
     *          // Unconditionally moves backward regardless of e4's direction
     *          e4.stepBackward(1);
     *          // For reverse traversal, you must step first
     *          ASSERT_EQ(*e4, expects[n]);
     *      }
     *  @endcode
     */
    class _nout cpIter {
        BY(ME(cpIter))

    public:
        /**
         * @brief Constructs iterator from pointer range
         * @note Direction automatically determined: reverse if begin > from, forward otherwise
         */
        cpIter(const nchar* begin, const nchar* from);

        /**
         * @brief Constructs iterator from string reference
         * @param from String must outlive this iterator
         * @param isReversed true for reverse iteration (starts at end), false for forward
         */
        explicit cpIter(const std::string& from, nbool isReversed = false);

        cpIter(std::string&& from, nbool isReversed = false) = delete;

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

        /**
         * @brief Advances iterator in its default direction
         * @return Actual codepoints moved (may be less if end reached)
         * @note Moves forward for forward iterators, backward for reverse iterators
         */
        ncnt next(ncnt step);

        /**
         * @brief Unconditionally advances iterator forward
         * @return Actual codepoints moved (may be less if end reached)
         * @note Direction is absolute, unaffected by iterator's reverse flag
         */
        ncnt stepForward(ncnt step);

        /**
         * @brief Unconditionally advances iterator backward
         * @return Actual codepoints moved (may be less if beginning reached)
         * @note Direction is absolute, unaffected by iterator's reverse flag
         */
        ncnt stepBackward(ncnt step);

        std::string get() const;
        ncnt remainLen() const;

    private:
        ncnt _step(const std::function<const nchar*()>& closure, ncnt step);
        const nchar* _nextCodepoint(const nchar* from) const;
        const nchar* _prevCodepoint(const nchar* e) const;
        ncnt _skipBytes(nchar ch) const;

    private:
        const nchar* _begin;
        const nchar* _end;
        nbool _isReversed;
    };
} // namespace by
