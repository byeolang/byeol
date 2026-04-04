#include "frontend/flag/flags/dirFlag.hpp"
#include "frontend/flag/flags/fileFlag.hpp"

namespace by {
    BY_DEF_ME(dirFlag)

    const nchar* me::getName() const { return "<directory path>"; }

    const nchar* me::getDescription() const {
        return R"DESC(
    directory path including byeol source code files.
    the path is basically relative to current working directory but you can use
    absoulte path beginning with `/`.
    this automatically traverses all sub folders to find `.byeol` files.)DESC";
    }

    const strings& me::_getRegExpr() const {
        static strings inner{"[^\\@\\#\\$\\%\\[\\]\\+\\\\\\|\\<\\>\\=\\!\\^\\&\\*\\(\\)\\{\\}\\n\\s]+"};
        return inner;
    }

    me::res me::_onTake(const flagArgs& tray, cli&, interpreter& ip, starter&, errReport& rpt) const {
        parser& ps = ip.getParser();
        tnarr<srcSupply> supplies;
        for(const auto& dirPath: tray)
            supplies.add(srcSupply::makeSuppliesFrom(dirPath.get()));

        ps.addSupply(supplies);
        if(supplies.isEmpty()) rpt.add(nerr::newErr(NO_SRC_FOUND_ON_THE_PATH, util::joinVectorString(tray).c_str()));
        return MATCH;
    }
}
