#pragma once

#include "sobj.hpp"

namespace nm {

    class _nout termSobj : public sobj {
        NAMU(ME(termSobj, sobj),
            CLONE(me),
            INIT_META(termSobj))

    public:
        termSobj(const std::string& rawVal, const std::string& name = "");
        termSobj(nbool val, const std::string& name = "");
        termSobj(nint val, const std::string& name = "");
        termSobj(nflt val, const std::string& name = "");
        termSobj(const nchar* val, const std::string& name = "");
        termSobj(const nchar val, const std::string& name = "");
        termSobj(const me& rhs, const std::string& name = "");

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
