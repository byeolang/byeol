/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/frame/frameInteractable.hpp"
#include "core/type/ntype.hpp"
#include "core/builtin/container/native/tnmap.hpp"
#include "core/ast/tpriorities.hpp"
#include "core/ast/validable.hpp"

namespace by {

    class ases;
    class args;
    class visitor;
    class visitInfo;
    class baseObj;
    class src;
    class modifier;
    template <typename K, typename V, typename defaultContainer> class tnchain;

    typedef tnchain<std::string, node, tnmap<std::string, node, immutableTactic>> scope;

    template <typename T> class tmock;

    /// @ingroup core
    /// @brief Base class for all AST nodes in byeol language
    /// @details Provides common API to manipulate sub nodes and handle
    /// the hierarchical structure of the byeol abstract syntax tree.
    class _nout node: public instance, public frameInteractable {
        BY(ADT(node, instance))
        friend class exprMaker; // for _setSrc
        template <typename T> friend class tmock;

    public:
        me& operator[](const std::string& name);
        const me& operator[](const std::string& name) const BY_CONST_FUNC(operator[](name));
        me& operator[](const std::string* it);
        const me& operator[](const std::string* it) const BY_CONST_FUNC(operator[](it));
        me& operator[](const nchar* name);
        const me& operator[](const nchar* name) const BY_CONST_FUNC(operator[](name));

    public:
        nbool in(const node& elem) const;
        nbool in(const node* elem) const;

        virtual scope& subs() = 0;
        const scope& subs() const BY_CONST_FUNC(subs())
        virtual tstr<nbicontainer> mySubs() const;

        /// @param r type to be deduced to this type.
        ///          if a type is null, it means that there is no type specified.
        ///          it's complete different to 'void' type.
        /// @return null if it's not relative between l & r.
        const node* deduce(const node& it) const;
        const node* deduce(const node* it) const BY_SIDE_FUNC(deduce);

        template <typename T> T* sub(std::function<nbool(const std::string&, const T&)> l);
        template <typename T>
        const T* sub(std::function<nbool(const std::string&, const T&)> l) const BY_CONST_FUNC(sub<T>(l))
        template <typename T = me> T* sub();
        template <typename T = me> const T* sub() const BY_CONST_FUNC(sub<T>())
        template <typename T = me> T* sub(const std::string& name);
        template <typename T = me> T* sub(const std::string* it) BY_SIDE_FUNC(sub);
        template <typename T = me> const T* sub(const std::string& name) const BY_CONST_FUNC(sub<T>(name))
        template <typename T = me> const T* sub(const std::string* name) const BY_CONST_FUNC(sub<T>(name))
        template <typename T = me> T* sub(const std::string& name, const args& a);
        template <typename T = me>
        T* sub(const std::string* name, const args& a) BY_SIDE_FUNC(name, sub(*name, a), nullptr);
        template <typename T = me>
        const T* sub(const std::string& name, const args& a) const BY_CONST_FUNC(sub<T>(name, a))
        template <typename T = me>
        const T* sub(const std::string* name, const args& a) const BY_CONST_FUNC(sub<T>(name, a))

        template <typename T> tnarr<T, strTactic> subAll(std::function<nbool(const std::string&, const T&)> l) const;
        template <typename T = me> tnarr<T, strTactic> subAll() const;
        template <typename T = me> tpriorities<T> subAll(const std::string& name) const;
        template <typename T = me>
        tpriorities<T> subAll(const std::string* it) const BY_SIDE_FUNC(subAll<T>)


            /// finds subnodes of this node, allowing implicit conversion based on name and
            /// arguments.
            /// @param a if you pass this as a nullptr, the argument will be ignored and all names
            /// that are the same
            ///          will be considered to meet the condition.
            /// @warning if you pass the argument as `args()`, it will be considered to meet the
            ///          condition only if there are no parameters and the names are the same, so
            ///          the two have completely different meanings.
            template <typename T = me>
            tpriorities<T> subAll(const std::string& name, const args* a) const;
        template <typename T = me> tpriorities<T> subAll(const std::string& name, const args& a) const;
        template <typename T = me>
        tpriorities<T> subAll(const std::string* name, const args* a) const
            BY_SIDE_FUNC(name, subAll<T>(*name, a), tpriorities<T>());
        template <typename T = me>
        tpriorities<T> subAll(const std::string* name, const args& a) const
            BY_SIDE_FUNC(name, subAll<T>(*name, a), tpriorities<T>());

        bool canRun(const args& a) const;
        virtual priorType prioritize(const args& a) const = 0;
        priorType prioritize(const args* it) const BY_SIDE_FUNC(it, prioritize(*it), NO_MATCH);

        virtual str run(const args& a) = 0;
        str run(const args* it) BY_SIDE_FUNC(run);
        str run(const std::string& name, const args& a);
        str run(const std::string& name, const args* a) BY_SIDE_FUNC(a, run(name, a), str());
        str run(const std::string* name, const args& a) BY_SIDE_FUNC(name, run(*name, a), str());
        str run(const std::string* name, const args* a) BY_SIDE_FUNC(name&& a, run(*name, *a), str());
        str run(const std::string& name);
        str run(const std::string* it) BY_SIDE_FUNC(run);
        str run();

        /// release all holding resources and ready to be terminated.as(
        /// @remark some class won't be able to reinitialize after rel() got called.
        virtual void rel() {}

        template <typename T> nbool is() const { return is(ttype<T>::get()); }

        nbool is(const typeProvidable& to) const;
        nbool is(const typeProvidable* it) const BY_SIDE_FUNC(is);
        nbool is(const type& to) const;
        nbool is(const type* it) const BY_SIDE_FUNC(is);

        template <typename T> tstr<T> as() const { return as(ttype<T>::get()); }

        str as(const typeProvidable& to) const;
        str as(const typeProvidable* it) const BY_SIDE_FUNC(as);
        str as(const type& to) const;
        str as(const type* it) const BY_SIDE_FUNC(as);

        template <typename T> nbool isImpli() const { return isImpli(ttype<T>::get()); }

        virtual nbool isImpli(const type& to) const;
        nbool isImpli(const typeProvidable& to) const;
        nbool isImpli(const typeProvidable* it) const BY_SIDE_FUNC(isImpli);

        template <typename T> tstr<T> asImpli() const { return asImpli(ttype<T>::get()); }

        virtual str asImpli(const type& to) const;
        str asImpli(const typeProvidable& to) const;
        str asImpli(const typeProvidable* it) const BY_SIDE_FUNC(asImpli);

        virtual const src& getSrc() const;

        /// returns whether this instance is complete or not.
        /// if some instance is incomplete, it means that it's not accessible on byeolang source
        /// codes.
        virtual nbool isComplete() const;

        /// getType() returns what it is. opposite to it, this returns what this class will
        /// represents after evaluation.
        ///
        /// for example, the @ref expr class has derived from this node class. and if an user call the
        /// funcs to get type of it, class 'ntype' of 'expr' will be returned.
        /// but if that user call the 'getEval()' then the 'expr' object evaluate its terms and
        /// returns it as a node from the output. it could be an integer if it was @ref addExpr and all
        /// terms are constructed with integers.
        ///
        /// and also this func requires frames. means that you have to interact frame first before
        /// call this func.
        /// @remark some derived class of node may require to call this function first by @ref verifier.
        ///         if you call this function without calling @ref verifier previously will damage then
        ///         process.
        virtual str getEval() const;

        virtual void accept(const visitInfo& i, visitor& v);

        using frameInteractable::inFrame;
        void inFrame(const bicontainable* args) const override;
        using frameInteractable::outFrame;
        void outFrame() const override;

        virtual const modifier& getModifier() const;

    protected:
        virtual str _onRunSub(node& sub, const args& a);
        virtual void _setSrc(const src& s);
        void _setSrc(const src* it) BY_SIDE_FUNC(_setSrc);
    };

    extern template class tnmap<std::string, node>;
    typedef tnmap<std::string, node> nmap;
} // namespace by
