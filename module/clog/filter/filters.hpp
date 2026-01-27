/// @file
#pragma once

#include "clog/filter/filterable.hpp"

namespace by {
    /** @ingroup clog
     *  @brief Collection of filterable objects for logging control
     *  @details Manages multiple @ref filterable objects registered in the @ref logger class.
     *  Provides the same API as filterable, and calling an API on filters will invoke that API
     *  on all owned filterable objects.
     *
     *  Acts as a composite filter, allowing multiple filtering criteria to be applied to
     *  log messages simultaneously.
     */
    class _nout filters: public filterable {
        BY(ME(filters))

    public:
        filters() {}

        /**
         * @brief Constructs a filters object with multiple filterable elements.
         * @details This variadic template constructor allows convenient initialization
         *          of the filters object by adding multiple filterable instances.
         * @tparam Es Type of the filterable elements (must be convertible to filterable*).
         * @param elems Pointers to the filterable objects to add.
         */
        template <typename... Es> explicit filters(const Es*... elems) { (add(elems), ...); }

        virtual ~filters();

    public:
        ncnt len() const;
        /**
         * @brief Clears all contained filterable objects without deleting them.
         */
        void rel();
        /**
         * @brief Adds a filterable object to the collection.
         * @param new1 The filterable object to add. The filters object does not take ownership.
         */
        void add(const filterable& new1);
        /**
         * @brief Adds a filterable object to the collection.
         * @param new1 Pointer to the filterable object to add. The filters object does not take ownership.
         */
        void add(const filterable* new1);
        /**
         * @brief Deletes all owned filterable objects and clears the collection.
         * @note This method assumes ownership of the added filterable pointers and deletes them.
         */
        void del();
        /**
         * @brief Applies all contained filters to the log entry.
         * @details This method returns true if *any* of the contained filterable objects
         *          return true for the given log entry, indicating it should be filtered out.
         * @param lv The error level of the log entry.
         * @param tag The tag associated with the log entry.
         * @return true if the log entry should be filtered out (blocked), false otherwise.
         */
        nbool filt(errLv::level lv, const std::string& tag) const;

    private:
        std::vector<const filterable*> _arr;
    };
}
