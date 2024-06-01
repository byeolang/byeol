#pragma once

#include "validable.hpp"
#include "../builtin/container/native/tnbicontainer.hpp"
#include "../builtin/container/native/tnarr.hpp"
#include "point.hpp"
#include "../type/ntype.hpp"
#include "../frame/frameInteractable.hpp"
#include "tpriorities.hpp"

namespace namu {

    class ases;
    class args;
    class visitor;
    struct visitInfo;
    class baseObj;
    class src;

    /// node provides common API to manipulate its sub nodes.
    class _nout node : public instance, public frameInteractable, public exceptional {
        NAMU(ADT(node, instance))
        friend class exprMaker; // for _setSrc

    public:
        node& operator[](const std::string& name);
        const node& operator[](const std::string& name) const NAMU_UNCONST_FUNC(operator[](name))

    public:
        nbool has(const node& elem) const;
        nbool has(const node* elem) const;

        virtual nbicontainer& subs() = 0;
        const nbicontainer& subs() const NAMU_UNCONST_FUNC(subs())

        /// @return null if it's not relative between l & r.
        const node& deduce(const node& r) const {
            if(nul(r)) return nulOf<node>();
            const ntype& ltype = getType();
            const ntype& rtype = r.getType();
            const ntype& res = ltype.deduce(rtype);
            if(nul(res)) return nulOf<node>();
            if(res == ltype) return *this;
            if(res == rtype) return r;

            return nulOf<node>();
        }

        template <typename T>
        T& sub(std::function<nbool(const std::string&, const T&)> l) {
            return subs().get<T>(l);
        }
        template <typename T>
        const T& sub(std::function<nbool(const std::string&, const T&)> l) const NAMU_UNCONST_FUNC(sub<T>(l))
        template <typename T = me> T& sub();
        template <typename T = me> const T& sub() const NAMU_UNCONST_FUNC(sub<T>())
        template <typename T = me> T& sub(const std::string& name);
        template <typename T = me> const T& sub(const std::string& name) const NAMU_UNCONST_FUNC(sub<T>(name))
        template <typename T = me> T& sub(const std::string& name, const args& a);
        template <typename T = me> const T& sub(const std::string& name, const args& a) const NAMU_UNCONST_FUNC(sub<T>(name, a))

        template <typename T>
        tnarr<T, strTactic> subAll(std::function<nbool(const std::string&, const T&)> l) const {
            return subs().getAll<T>(l);
        }

        template <typename T = me> tnarr<T, strTactic> subAll() const;
        template <typename T = me> tpriorities<T> subAll(const std::string& name) const;
        template <typename T = me> tpriorities<T> subAll(const std::string& name, const args& a) const;

        bool canRun(const args& a) const;
        virtual priority prioritize(const args& a) const = 0;

        virtual str run(const args& a) = 0;
        str run(const std::string& name, const args& a);
        str run(const std::string& name);
        str run();

        /// release all holding resources and ready to be terminated.as(
        /// @remark some class won't be able to reinitialize after rel() got called.
        virtual void rel() {}

        template <typename T> nbool is() const { return is(ttype<T>::get()); }
        nbool is(const typeProvidable& to) const;
        nbool is(const type& to) const;

        template <typename T> tstr<T> as() const { return as(ttype<T>::get()); }
        str as(const typeProvidable& to) const;
        str as(const type& to) const;

        template <typename T>
        nbool isImpli() const { return isImpli(ttype<T>::get()); }
        nbool isImpli(const typeProvidable& to) const;
        virtual nbool isImpli(const type& to) const;

        template <typename T> tstr<T> asImpli() const { return asImpli(ttype<T>::get()); }
        str asImpli(const typeProvidable& to) const;
        virtual str asImpli(const type& to) const;

        virtual const src& getSrc() const;

        clonable* cloneDeep() const override;

        /// returns whether this instance is complete or not.
        /// if some instance is incomplete, it means that it's not accessible on namulang source
        /// codes.
        virtual nbool isComplete() const;

        /// getType() returns what it is. opposite to it, this returns what this class will
        /// represents after evaluation.
        ///
        /// for example, the 'expr' class has derived from this node class. and if an user call the
        /// funcs to get type of it, class 'ntype' of 'expr' will be returned.
        /// but if that user call the 'getEval()' then the 'expr' object evaluate its terms and
        /// returns it as a node from the output. it could be an integer if it was 'addExpr' and all
        /// terms are constructed with integers.
        ///
        /// and also this func requires frames. means that you have to interact frame first before
        /// call this func.
        virtual str getEval() const;

        virtual void accept(visitInfo i, visitor& v);

        using frameInteractable::inFrame;
        void inFrame(const bicontainable& args) override;
        void outFrame() override;

    protected:
        virtual str _onRunSub(node& sub, const args& a);
        virtual void _setSrc(const src& s);

    private:
    };
}
