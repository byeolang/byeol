/// @file
#pragma once

#include "core/ast/pack.hpp"

namespace by {

    class errReport;
    class packLoading;
    typedef std::vector<packLoading*> packLoadings;

    /** @ingroup core
     *  @brief Pack loading and management system
     *  @details Manages the loading of slots from manifest files and file paths.
     *  Coordinates pack loading strategies and maintains slot hierarchies for the runtime system.
     */
    class _nout packLoader: public typeProvidable, public clonable {
        BY(CLASS(packLoader))

    public:
        packLoader();

    public:
        me& addPath(const std::string& filePath);
        me& addPath(const std::vector<std::string> paths);
        me& addPath(std::initializer_list<const nchar*> paths);
        me& addRelativePath(const std::string& path);
        /**
         * @brief Sets the map where loaded packs will be stored.
         * @param s The `nmap` object to store the loaded packs.
         * @return A reference to this `packLoader` instance.
         */
        me& setBasePacks(nmap& s);
        me& setBasePacks(nmap* it) BY_SIDE_FUNC(it, setBasePacks(*it), *this);

        /**
         * @brief Initiates the pack loading process.
         * @details This method triggers the discovery, parsing, verification, and linking
         *          of all packs found in the configured paths.
         */
        void load();

    private:
        void _makePacks(nmap& tray);
        void _addNewPack(nmap& tray, manifest& mani, const std::string& dirPath);

        void _logPack(const pack& pak) const;

        manifest _interpManifest(const std::string& dir, const std::string& manPath) const;

        packLoading* _makeLoading(const std::string& name) const;
        const packLoadings& _getLoadings() const;

    private:
        tstr<nmap> _packs;
        std::vector<std::string> _paths;
        static constexpr nchar DELIMITER = '/';
    };
} // namespace by
