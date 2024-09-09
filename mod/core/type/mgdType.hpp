#pragma once

#include "../ast/obj.hpp"
#include "ntype.hpp"

namespace nm {

    class node;
    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<node, strTactic> narr;

    class _nout mgdType: public ttype<baseObj>, public clonable {
        NM(ME(mgdType, ttype<baseObj>), CLONE(mgdType))

    public:
        mgdType(const std::string& name, const type& super);
        mgdType(const std::string& name, const type& super, const params& ps);
        mgdType(const std::string& name, const types& supersFromRhs);

    public:
        const type& getSuper() const override;
        const nbool& isInit() const override;
        const std::string& getName() const override;

        // TODO: getSubs(), getLeafs()

        template <typename S> static mgdType make(const std::string& name) {
            return mgdType(name, ttype<S>::get());
        }

        static mgdType make(const std::string& name);

        template <typename S> static mgdType* makeNew(const std::string& name) {
            return new mgdType(name, ttype<S>::get());
        }

        static mgdType* makeNew(const std::string& name);

    protected:
        types& _getSupers() override;

    private:
        void _initSupers(const type& super);

    private:
        std::string _name;
        types _supers;
    };
} // namespace nm
