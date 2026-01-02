/// @file
#pragma once

#include "clog/filter/filterable.hpp"

namespace by {
    /// @ingroup clog
    /// @brief Collection of filterable objects for logging control
    /// @details Manages multiple @ref filterable objects registered in the @ref logger class.
    /// Provides the same API as filterable, and calling an API on filters will invoke that API
    /// on all owned filterable objects.
    ///
    /// Acts as a composite filter, allowing multiple filtering criteria to be applied to
    /// log messages simultaneously.
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
