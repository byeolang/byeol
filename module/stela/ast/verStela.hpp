/// @file
#pragma once

#include "stela/ast/valStela.hpp"

namespace by {

    /// @ingroup stela
    /// @brief Version stela node for semantic version handling
    /// @details Specialized value stela for storing and comparing semantic versions.
    /// Supports major.minor.fix version format with comparison operations.
    class _nout verStela: public valStela {
        BY_ME(verStela, valStela)
        BY_INIT_META(verStela)

    public:
        verStela(int major, int minor, int fix);
        explicit verStela(const std::string& verStr);
        explicit verStela(const nchar* verStr);

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
