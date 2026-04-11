/// @file
#pragma once

#include "core/ast/pod.hpp"

namespace by {

    class errReport;
    class podLoading;
    typedef std::vector<podLoading*> podLoadings;

    /** @ingroup core
     *  @brief Pod loading and management system
     *  @details Manages the loading of slots from manifest files and file paths.
     *  Coordinates pod loading strategies and maintains slot hierarchies for the runtime system.
     */
    class _nout podLoader: public typeProvidable, public clonable {
        BY(CLASS(podLoader))

    public:
        podLoader();

    public:
        me& addPath(const std::string& filePath);
        me& addPath(const std::vector<std::string> paths);
        me& addPath(std::initializer_list<const nchar*> paths);
        me& addRelativePath(const std::string& path);
        /**
         * @brief Sets the map where loaded pods will be stored.
         * @param s The `nmap` object to store the loaded pods.
         * @return A reference to this `podLoader` instance.
         */
        me& setBasePods(nmap& s);
        me& setBasePods(nmap* it) BY_SIDE_FUNC(it, setBasePods(*it), *this);

        /**
         * @brief Initiates the pod loading process.
         * @details This method triggers the discovery, parsing, verification, and linking
         *          of all pods found in the configured paths.
         */
        void load();

    private:
        void _makePods(nmap& tray);
        void _addNewPod(nmap& tray, manifest& mani, const std::string& dirPath);

        void _logPod(const pod& pak) const;

        manifest _interpManifest(const std::string& dir, const std::string& manPath) const;

        podLoading* _makeLoading(const std::string& name) const;
        const podLoadings& _getLoadings() const;

    private:
        tstr<nmap> _pods;
        std::vector<std::string> _paths;
        static constexpr nchar DELIMITER = '/';
    };
} // namespace by
