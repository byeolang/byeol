#pragma once

#include "tas.hpp"

namespace nm {

    class _nout ases: public asable {
    public:
        ~ases() override;

    public:
        void add(const asable& newCast);
        void add(const asable* newCast);

        int len() const;

        nbool is(const type& from, const type& to) const override;

        str as(const node& from, const type& to) const override;

        void rel();

    private:
        asables _casts;
    };
}
