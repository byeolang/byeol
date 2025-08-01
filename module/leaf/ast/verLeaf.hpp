/// @file
#pragma once

#include "leaf/ast/valLeaf.hpp"

namespace by {

    /// @ingroup leaf
    /// @brief Version leaf node for semantic version handling
    /// @details Specialized value leaf for storing and comparing semantic versions.
    /// Supports major.minor.fix version format with comparison operations.
    class _nout verLeaf: public valLeaf {
        BY_ME(verLeaf, valLeaf)
        BY_INIT_META(verLeaf)

    public:
        verLeaf(int major, int minor, int fix);
        explicit verLeaf(const std::string& verStr);
        explicit verLeaf(const nchar* verStr);

    public:
        nbool operator>(const me& rhs) const;
        nbool operator<(const me& rhs) const;
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;
        nbool operator<=(const me& rhs) const;
        nbool operator>=(const me& rhs) const;

    public:
        nint asMajor() const;
        nint asMinor() const;
        nint asFix() const;
        const type& getType() const override;

    private:
        static nint _isFromBigger(nint from, nint to);
        void _parseVerStr(const std::string& verStr);

    private:
        nint _maj;
        nint _min;
        nint _fix;
        static constexpr nint VER_LEN = 3;
        static constexpr const nchar* DELIMITER = ".";
    };
} // namespace by
