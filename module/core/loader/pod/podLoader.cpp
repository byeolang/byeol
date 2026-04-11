#include "core/loader/pod/podLoader.hpp"

#include "core/ast/autopod.hpp"
#include "core/frame/thread.hpp"
#include "core/builtin/err/errReport.hpp"
#include "core/loader/pod/podLoading.hpp"

namespace by {

    BY_DEF_ME(podLoader)

    me::podLoader() {}

    void me::load() {
        // TODO: returns result when it's fail
        if(!_pods) _pods.bind(new nmap());

        _makePods(*_pods);
    }

    manifest me::_interpManifest(const std::string& dir, const std::string& manPath) const {
        // TODO: open pod zip file -> extract manifest.stela file -> interpret it & load values
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

    const podLoadings& me::_getLoadings() const {
        static podLoadings* inner = nullptr;
        if(!inner) {
            inner = new podLoadings();
            for(const type* sub: ttype<podLoading>::get().getLeafs()) {
                podLoading* new1 = sub->makeAs<podLoading>();
                if(!new1) {
                    BY_E("fail to make podMaking named to %s", sub->getName());
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
        BY_I("finding pods relative to %s or absolute", cwd);
        return addPath(cwd + path);
    }

    me& me::setBasePods(nmap& s) {
        _pods.bind(s);
        return *this;
    }

    void me::_makePods(nmap& tray) {
        for(const std::string& path: _paths) {
            BY_I("try pod path: %s", path);

            auto e = fsystem::find(path);
            while(e.next()) {
                if(e.getName() != MANIFEST_FILENAME) continue;

                auto manifestPath = e.getDir() + fsystem::getDelimiter() + e.getName();
                manifest mani = _interpManifest(e.getDir(), manifestPath);
                if(tray.in(mani.name)) { // TODO: more concise duplication check required including version of pod.
                    BY_W("pod '%s' is duplicated. an existed one will be used.", mani.name);
                    continue;
                }

                _addNewPod(tray, mani, e.getDir());
            }
        }
    }

    void me::_addNewPod(nmap& tray, manifest& mani, const std::string& dirPath) {
        BY_I("manifest path: %s", mani.filePath);

        podLoadings loadings;
        for(entrypoint& point: mani.points) {
            podLoading* newLoading = _makeLoading(point.lang);
            if(!newLoading) {
                BY_W("%s language not supported for loading %s pod.", mani.points[0].lang, mani.name);
                continue;
            }

            strings fullPaths;
            for(const std::string& path: point.paths)
                fullPaths.push_back(dirPath + path);

            newLoading->addPath(fullPaths);
            loadings.push_back(newLoading);
        }

        pod* new1 = new autopod(mani, loadings);
        tray.add(mani.name, new1);
        _logPod(*new1);
    }

    void me::_logPod(const pod& p) const {
        BY_I("new pod [%s] has been added.", p.getManifest().name);

#if BY_IS_DBG
        const manifest& mani = p.getManifest();
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

    podLoading* me::_makeLoading(const std::string& name) const {
        for(const podLoading* e: _getLoadings())
            if(e->getName() == name) return (podLoading*) e->clone();

        BY_E("can't find exact podLoading like %s", name);
        return nullptr;
    }
} // namespace by
