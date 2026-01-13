/// @file
#pragma once

#include "core/type/as/tas.hpp"
#include "core/builtin/container/native/tnarr.hpp"

namespace by {

    /** @ingroup core
     *  @brief Collection of type converters
     *  @details Container for managing multiple type conversion strategies.
     */
    class _nout ases: public tnarr<aser>, public asable {
        BY(CLASS(ases, tnarr<aser>))

    public:
        ases();

        template <typename... Es> explicit ases(const Es&... elems): super(elems...) {}

    public:
        using asable::is;
        nbool is(const type& from, const type& to) const override;

        using asable::as;
        str as(const node& from, const type& to) const override;
    };
}
