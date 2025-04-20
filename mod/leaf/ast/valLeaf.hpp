#pragma once

#include "leaf/ast/leaf.hpp"

namespace nm {

    class _nout valLeaf: public leaf {
        NM(ME(valLeaf, leaf), CLONE(me), INIT_META(valLeaf))

    public:
        valLeaf(const std::string& rawVal, const std::string& name = "");
        valLeaf(nbool val, const std::string& name = "");
        valLeaf(nint val, const std::string& name = "");
        valLeaf(nflt val, const std::string& name = "");
        valLeaf(const me& rhs, const std::string& name = "");

    public:
        const std::string& asStr() const override;
        nchar asChar() const override;
        nint asInt() const override;
        nbool asBool() const override;
        const type& getType() const override;

    private:
        static std::string toLower(std::string it);

    private:
        std::string _rawVal;
    };
}
