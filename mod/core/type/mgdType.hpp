#pragma once

#include "ntype.hpp"
#include "../ast/obj.hpp"

namespace nm {

    class node;
    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<node, strTactic> narr;

    class _nout mgdType : public ttype<obj>, public clonable {
        NM(ME(mgdType, ttype<obj>),
           CLONE(mgdType))

    public:
        mgdType(const std::string& name);
        mgdType(const std::string& name, const mgdType& super);
        mgdType(const std::string& name, const narr& bean);
        mgdType(const std::string& name, const mgdType& super, const narr& bean);
        mgdType(const std::string& name, const types& supersFromRhs);

    public:
        const type& getSuper() const override;
        const nbool& isInit() const override;
        const std::string& getName() const override;

        // TODO: getSubs(), getLeafs()

    protected:
        types& _getSupers() override;

    private:
        void _initSupers(const type& super);

    private:
        std::string _name;
        types _supers;
    };
}
