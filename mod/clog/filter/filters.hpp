#pragma once

#include "filterable.hpp"

namespace nm {
    class _nout filters : public filterable {
        NM(ME(filters))

    public:
        filters() {}

        template <typename... Es>
        explicit filters(const Es*... elems) {
            (add(elems),...);
        }

        virtual ~filters();

    public:
        ncnt len() const;
        void rel();
        void add(const filterable* new1);
        void del();
        std::string filt(logLv::level lv, const nchar* tag, const std::string& msg) const;

    private:
        std::vector<const filterable*> _arr;
    };
}
