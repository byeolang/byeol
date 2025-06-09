#pragma once

#include "core/ast/params.hpp"
#include "core/type/ntype.hpp"

namespace nm {

    class node;
    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<node, strTactic> narr;

    class _nout mgdType: public ntype, public clonable {
        NM(ME(mgdType, ntype), CLONE(mgdType))

    public:
        mgdType(const std::string& name, const type& super, const params& ps);
        mgdType(const std::string& name, const type& super, const params& ps, nbool isAdt,
            const node& ret);
        mgdType(const std::string& name, const types& supersFromRhs);

    private:
        mgdType(const std::string& name, const types& supers, const types& subs, const params& ps,
            nbool isAdt);

    public:
        nbool isTemplate() const override;
        nbool isAbstract() const override;
        const type& getSuper() const override;
        const nbool& isInit() const override;
        const std::string& getName() const override;

        ncnt size() const override;

        void onCloneDeep(const clonable& from) override;

        using super::getParams;
        params& getParams() override;

        const node* getRet() const override;
        void setRet(const node& new1) override;

        // TODO: getSubs(), getLeafs()

        void* make() const override;

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
        str _ret;
    };
} // namespace nm
