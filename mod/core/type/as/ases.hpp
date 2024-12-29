#pragma once

#include "tas.hpp"
#include "../../builtin/container/native/tnarr.hpp"

namespace nm {

    class _nout ases: public tnarr<aser>, public asable {
        NM(CLASS(ases, tnarr<aser>))

    public:
        nbool is(const type& from, const type& to) const override;

        str as(const node& from, const type& to) const override;
    };
}
