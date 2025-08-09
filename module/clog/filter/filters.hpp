#pragma once

#include "clog/filter/filterable.hpp"

namespace by {
    class _nout filters: public filterable {
        BY(ME(filters))

    public:
        filters() {}

        template <typename... Es> explicit filters(const Es*... elems) { (add(elems), ...); }

        virtual ~filters();

    public:
        ncnt len() const;
        void rel();
        void add(const filterable& new1);
        void add(const filterable* new1);
        void del();
        nbool filt(errLv::level lv, const std::string& tag) const;

    private:
        std::vector<const filterable*> _arr;
    };
}
