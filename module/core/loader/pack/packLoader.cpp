#include "core/loader/pack/packLoader.hpp"

#include "core/ast/autopack.hpp"
#include "core/frame/thread.hpp"
#include "core/builtin/err/errReport.hpp"
#include "core/loader/pack/packLoading.hpp"

namespace by {

    BY_DEF_ME(packLoader)

    me::packLoader() {}

    void me::load() {
        // TODO: returns result when it's fail
        if(!_packs) _packs.bind(new nmap());

        _makePacks(*_packs);
    }

    manifest me::_interpManifest(const std::string& dir, const std::string& manPath) const {
        // TODO: open pack zip file -> extract manifest.stela file -> interpret it & load values
        tstr<stela> loaded =
            stelaParser().parseFromFile(manPath) OR.err("error to load %s: interpretion err", manPath).ret(manifest());
        stela& root = *loaded;

        std::string name = root["name"].asStr();
        std::string ver = root["ver"].asStr();
        std::string author = root["author"].asStr();

        entrypoints points;
        stela& entrypoints = root["entrypoints"];
        WHEN(!entrypoints.isExist()) .err("entrypoints not defined.").ret(manifest());
        for(auto& pair: entrypoints) {
            const std::string& path = pair.second->sub("path").asStr();
            WHEN(path == "") .err("error to load %s: no entrypoint path", manPath).ret(manifest());

            // TODO: path should be multiple
            points.push_back(entrypoint{pair.first, {fsystem::getDelimiter() + path}});
        }

        // post: all data interpreted. merge to manifest.
        return manifest{name, manPath, author, ver, points};
    }

    const packLoadings& me::_getLoadings() const {
        static packLoadings* inner = nullptr;
        if(!inner) {
            inner = new packLoadings();
            for(const type* sub: ttype<packLoading>::get().getLeafs()) {
                packLoading* new1 = sub->makeAs<packLoading>();
                if(!new1) {
                    BY_E("fail to make packMaking named to %s", sub->getName());
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
        WHEN(&_paths == &paths) .ret(*this);

        _paths.insert(_paths.end(), paths.begin(), paths.end());
        return *this;
    }

    me& me::addPath(std::initializer_list<const nchar*> paths) {
        for(const nchar* e: paths)
            addPath(e);
        return *this;
    }

    me& me::addRelativePath(const std::string& path) {
        std::string cwd = fsystem::getCurrentDir() + fsystem::getDelimiter();
        BY_I("finding packs relative to %s or absolute", cwd);
        return addPath(cwd + path);
    }

    me& me::setBasePacks(nmap& s) {
        _packs.bind(s);
        return *this;
    }

    void me::_makePacks(nmap& tray) {
        for(const std::string& path: _paths) {
            BY_I("try pack path: %s", path);

            auto e = fsystem::find(path);
            while(e.next())
                if(e.getName() == MANIFEST_FILENAME) _addNewPack(tray, e.getDir(), e.getName());
        }
    }

    void me::_addNewPack(nmap& tray, const std::string& dirPath, const std::string& manifestName) {
        std::string manifestPath = dirPath + fsystem::getDelimiter() + manifestName;
        BY_I("manifest path: %s", manifestPath);

        manifest mani = _interpManifest(dirPath, manifestPath);
        WHEN(!mani.isValid()) .err("invalid manifest[%s] found.", manifestPath.c_str()).ret();

        packLoadings loadings;
        for(entrypoint& point: mani.points) {
            packLoading* newLoading = _makeLoading(point.lang);
            if(!newLoading) {
                BY_W("%s language not supported for loading %s pack.", mani.points[0].lang, mani.name);
                continue;
            }

            strings fullPaths;
            for(const std::string& path: point.paths)
                fullPaths.push_back(dirPath + path);

            newLoading->addPath(fullPaths);
            loadings.push_back(newLoading);
        }

        pack* new1 = new autopack(mani, loadings);
        tray.add(mani.name, new1);
        _logPack(*new1);
    }

    void me::_logPack(const pack& pak) const {
        BY_I("new pack [%s] has been added.", pak.getManifest().name);

#if BY_IS_DBG
        const manifest& mani = pak.getManifest();
        BY_DI("\t.filePath=%s", mani.filePath);
        BY_DI("\t.author=%s", mani.author);
        BY_DI("\t.ver=%s", mani.ver);

        BY_DI("\t.entrypoints:");
        for(const entrypoint& point: mani.points) {
            BY_DI("\t\t.lang=%s", point.lang);
            BY_DI("\t\t.paths=%s", point.paths[0]);
        }
#endif
    }

    packLoading* me::_makeLoading(const std::string& name) const {
        for(const packLoading* e: _getLoadings())
            if(e->getName() == name) return (packLoading*) e->clone();

        BY_E("can't find exact packLoading like %s", name);
        return nullptr;
    }
} // namespace by
