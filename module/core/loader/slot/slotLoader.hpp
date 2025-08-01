/// @file
#pragma once

#include "core/ast/slot.hpp"

namespace by {

    class errReport;
    class packLoading;
    typedef std::vector<packLoading*> packLoadings;

    /// @ingroup core
    /// @brief Slot loading and management system
    /// @details Manages the loading of slots from manifest files and file paths.
    /// Coordinates pack loading strategies and maintains slot hierarchies for the runtime system.
    class _nout slotLoader: public typeProvidable, public clonable {
        BY(CLASS(slotLoader))

    public:
        slotLoader();

    public:
        me& addPath(const std::string& filePath);
        me& addPath(const std::vector<std::string> paths);
        me& addPath(std::initializer_list<const nchar*> paths);
        me& addRelativePath(const std::string& path);
        me& setBaseSlots(nmap& s);
        me& setBaseSlots(nmap* it) BY_SIDE_FUNC(it, setBaseSlots(*it), *this);
        me& setReport(errReport& report);

        void load();

    private:
        void _makeSlots(nmap& tray);
        void _addNewSlot(nmap& tray, const std::string& dirPath, const std::string& manifestName);

        void _logSlot(const slot& pak) const;

        manifest _interpManifest(const std::string& dir, const std::string& manPath) const;

        packLoading* _makeLoading(const std::string& name) const;
        const packLoadings& _getLoadings() const;

    private:
        tstr<errReport> _report;
        tstr<nmap> _slots;
        std::vector<std::string> _paths;
        static constexpr nchar DELIMITER = '/';
    };
} // namespace by
