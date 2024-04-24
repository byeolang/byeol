#include "packLoader.hpp"
#include "../../frame/thread.hpp"
#include "../errReport.hpp"

namespace wrd {

    WRD_DEF_ME(packLoader)

    me::packLoader(): _baseSlots(nullptr), _report(dummyErrReport::singletone) {}

    tstr<slots> me::load() {
        // TODO: returns result when it's fail
        slots* ret = new slots();

        _makeSlots(*ret);
        return tstr<slots>(*ret);
    }

    manifest me::_interpManifest(const std::string& dir, const std::string& manPath) const {
        // TODO: open slot zip file -> extract manifest.swrd file -> interpret it & load values
        tstr<sobj> loaded = sinterpreter().interpFile(manPath);
        if(!loaded)
            return WRD_E("error to load %s: interpretion err", manPath.c_str()), manifest();

        std::string name = loaded->sub("name").asStr();
        std::string ver = loaded->sub("ver").asStr();
        std::string author = loaded->sub("author").asStr();

        entrypoints points;
        sobj& entrypoints = loaded->sub("entrypoints");
        for(auto& pair: entrypoints) {
            const std::string& path = dir + DELIMITER + pair.second->sub("path").asStr();
            if(nul(path))
                return WRD_E("error to load %s: no entrypoint path", manPath.c_str()), manifest();

            // TODO: path should be multiple
            points.push_back(entrypoint {pair.first, {path}});
        }

        // post: all data interpreted. merge to manifest.
        return manifest {name, manPath, author, ver, points};
    }

    const packLoadings& me::_getLoadings() const {
        static packLoadings* inner = nullptr;
        if(!inner) {
            inner = new packLoadings();
            for(const type* sub : ttype<packLoading>::get().getLeafs()) {
                packLoading* new1 = sub->makeAs<packLoading>();
                if(nul(new1)) {
                    WRD_E("fail to make slotMaking named to %s", sub->getName().c_str());
                    continue;
                }

                inner->push_back(new1);
            }
        }

        return *inner;
    }

    me& me::addPath(const std::string& filePath) {
        _paths.push_back(filePath);
        return *this;
    }
    me& me::addPath(const std::vector<std::string> paths) {
        if(&_paths == &paths) return *this;

        _paths.insert(_paths.end(), paths.begin(), paths.end());
        return *this;
    }
    me& me::addPath(std::initializer_list<const wchar*> paths) {
        for(const wchar* e : paths)
            addPath(e);
        return *this;
    }

    me& me::setReport(errReport& report) {
        _report.bind(report);
        return *this;
    }

    me& me::setBaseSlots(slots& basis) {
        _baseSlots = &basis;
        return *this;
    }

    void me::_makeSlots(slots& tray) {
        std::string cwd = fsystem::getCurrentDir() + "/";
        WRD_I("finding slots relative to %s or absolute", cwd.c_str());

        for(const std::string& path : _paths) {
            WRD_I("try slot path: %s", path.c_str());

            auto e = fsystem::find(cwd + path);
            while(e.next())
                if(e.getName() == MANIFEST_FILENAME)
                    _addNewSlot(tray, e.getDir(), e.getName());
        }
    }

    void me::_addNewSlot(slots& tray, const std::string& dirPath, const std::string& manifestName) {
        std::string manifestPath = dirPath + DELIMITER + manifestName;

        manifest mani = _interpManifest(dirPath, manifestPath);
        if(!mani.isValid()) {
            WRD_E("invalid manifest[%s] found.", manifestPath.c_str());
            return;
        }

        packLoadings loadings;
        for(entrypoint& point : mani.points) {
            packLoading* newLoading = _makeLoading(point.lang);
            if(!newLoading) {
                WRD_W("%s language not supported for loading %s slot.", mani.points[0].lang.c_str(), mani.name.c_str());
                continue;
            }

            newLoading->addPath(point.paths);
            loadings.push_back(newLoading);
        }

        slot* new1 = new autoslot(mani, loadings);
        tray.add(mani.name, new1);
        _logSlot(*new1);
    }

    void me::_logSlot(const slot& pak) const {
        WRD_I("new slot [%s] has been added.", pak.getManifest().name.c_str());

#if WRD_IS_DBG
        const manifest& mani = pak.getManifest();
        WRD_DI("\t.filePath=%s", mani.filePath.c_str());
        WRD_DI("\t.author=%s", mani.author.c_str());
        WRD_DI("\t.ver=%s", mani.ver.c_str());

        WRD_DI("\t.entrypoints=");
        for(const entrypoint& point : mani.points) {
            WRD_DI("\t\t.lang=%s", point.lang.c_str());
            WRD_DI("\t\t.paths=%s", point.paths[0].c_str());
        }
#endif
    }

    packLoading* me::_makeLoading(const std::string& name) const {
        for(const packLoading* e : _getLoadings())
            if(e->getName() == name)
                return (packLoading*) e->clone();

        WRD_E("can't find exact packLoading like %s", name.c_str());
        return nullptr;
    }
}
