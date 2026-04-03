#include "core/loader/pack/byeol/byeolPackLoading.hpp"
#include "core/worker/parser.hpp"
#include "core/ast/src/src.hpp"
#include "core/worker/supply/fileSupply.hpp"
#include "core/ast/pack.hpp"
#include "core/frame/threadUse.hpp"
#include "core/worker/visitor/expander.hpp"

namespace by {

    BY_DEF_ME(byeolPackLoading)

    nbool me::parse(errReport& rpt, pack& pak) {
        parser ps;
        ps.setTask(pak).setReport(rpt);

        const auto& paths = _getPaths();
        for(const auto& path : paths)
            ps.addSupply(srcSupply::makeSuppliesFrom(path));

        str res = ps.work();
        return res.isBind();
    }

    nbool me::expand(errReport& rpt, pack& pak) {
        threadUse thr(rpt);
        expander ep;

        ncnt count = rpt.len();
        ep.setTask(pak).setReport(rpt).work();
        return rpt.len() == count; // success if no change.
    }

    const std::string& me::getName() const {
        static std::string inner = "byeol";
        return inner;
    }
}
