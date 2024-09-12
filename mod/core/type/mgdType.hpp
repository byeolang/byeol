#pragma once

#include "../ast/obj.hpp"
#include "../ast/params.hpp"
#include "ntype.hpp"

namespace nm {

    class node;
    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<node, strTactic> narr;

    class _nout mgdType: public ntype, public clonable {
        NM(ME(mgdType, ntype), CLONE(mgdType))

    public:
        mgdType(const std::string& name, const type& super);
        mgdType(const std::string& name, const type& super, const params& ps);
        mgdType(const std::string& name, const type& super, const params& ps, nbool isAdt);
        mgdType(const std::string& name, const types& supersFromRhs);

    public:
        nbool isTemplate() const override;
        nbool isAbstract() const override;
        const type& getSuper() const override;
        const nbool& isInit() const override;
        const std::string& getName() const override;

        void* make() const override;

        ncnt size() const override;

        using super::getParams;
        params& getParams() override;

        // TODO: getSubs(), getLeafs()

        template <typename S> static mgdType make(const std::string& name) {
            return mgdType(name, ttype<S>::get());
        }

        using super::make;
        static mgdType make(const std::string& name);

        template <typename S> static mgdType* makeNew(const std::string& name) {
            return new mgdType(name, ttype<S>::get());
        }

        static mgdType* makeNew(const std::string& name);

    protected:
        types& _getSupers() override;
        types& _getSubs() override;
        type& _getStatic() const override;
        types** _onGetLeafs() const override;

    private:
        void _initSupers(const type& super);

    private:
        std::string _name;
        types _supers;
        types _subs;
        params _params;
        nbool _isAdt;
    };
} // namespace nm
