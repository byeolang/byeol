#pragma once

#include "../../ast/slot.hpp"
#include "../pack/packLoading.hpp"

namespace nm {

    class errReport;

    class _nout slotLoader : public typeProvidable, public clonable {
        NM(CLASS(slotLoader))

    public:
        slotLoader();

    public:
        me& addPath(const std::string& filePath);
        me& addPath(const std::vector<std::string> paths);
        me& addPath(std::initializer_list<const nchar*> paths);
        me& addRelativePath(const std::string& path);
        me& setBaseSlots(nmap& s);
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
}
