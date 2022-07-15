#pragma once

#include "clonable.hpp"
#include "validable.hpp"
#include "../builtin/container/native/tnbicontainer.hpp"
#include "../builtin/container/native/tnarr.hpp"
#include "point.hpp"
#include "../type/wtype.hpp"
#include "../frame/frameInteractable.hpp"

namespace wrd {

    class ases;

    /// node provides common API to manipulate its sub nodes.
    class _wout node : public instance, public clonable, public frameInteractable {
        WRD(ADT(node, instance))

    public:
        node& operator[](const std::string& name) const;

    public:
        virtual nbicontainer& subs() = 0;
        const nbicontainer& subs() const WRD_UNCONST_FUNC(subs())

        /// @return null if it's not relative between l & r.
        const node& reduce(const node& r) const {
            const wtype& mine = getType();
            const wtype& res = mine.reduce(r.getType());
            if(nul(res) || nul(mine)) return nulOf<node>();

            return res == mine ? *this : r;
        }

        template <typename T>
        T& sub(std::function<wbool(const std::string&, const T&)> l) const {
            return subs().get<T>(l);
        }
        template <typename T = me> T& sub() const;
        template <typename T = me> T& sub(const std::string& name) const;
        template <typename T = me> T& sub(const std::string& name, const ucontainable& args);
        template <typename T = me> T& sub(const std::string& name, const ucontainable& args) const;

        template <typename T>
        tnarr<T> subAll(std::function<wbool(const std::string&, const T&)> l) const {
            return subs().getAll<T>(l);
        }

        template <typename T = me> tnarr<T, strTactic> subAll() const;
        template <typename T = me> tnarr<T, strTactic> subAll(const std::string& name) const;
        template <typename T = me> tnarr<T, strTactic> subAll(const std::string& name, const ucontainable& args);
        template <typename T = me> tnarr<T, strTactic> subAll(const std::string& name, const ucontainable& args) const;

        virtual wbool canRun(const ucontainable& args) const = 0;

        virtual str run(const ucontainable& args) = 0;
        str run(const std::string& name, const ucontainable& args);
        str run(const std::string& name);
        str run();

        /// release all holding resources and ready to be terminated.as(
        /// @remark some class won't be able to reinitialize after rel() got called.
        virtual void rel() {}

        template <typename T> wbool is() const { return is(ttype<T>::get()); }
        wbool is(const typeProvidable& to) const;
        wbool is(const type& to) const;

        template <typename T> tstr<T> as() const { return as(ttype<T>::get()); }
        str as(const typeProvidable& to) const;
        str as(const type& to) const;

        template <typename T>
        wbool isImpli() const { return isImpli(ttype<T>::get()); }
        wbool isImpli(const typeProvidable& to) const;
        virtual wbool isImpli(const type& to) const;

        template <typename T> tstr<T> asImpli() const { return asImpli(ttype<T>::get()); }
        str asImpli(const typeProvidable& to) const;
        virtual str asImpli(const type& to) const;

        virtual const point& getPos() const {
            static point inner;
            return inner;
        }

        virtual void setPos(const point& new1) {}

        /// getType() returns what it is. opposite to it, this returns what this class will
        /// represents after evaluation.
        ///
        /// for example, the 'expr' class has derived from this node class. and if an user call the
        /// funcs to get type of it, class 'wtype' of 'expr' will be returned.
        /// but if that user call the 'getEval()' then the 'expr' object evaluate its terms and
        /// returns it as a node from the output. it could be an integer if it was 'addExpr' and all
        /// terms are constructed with integers.
        ///
        /// and also this func requires frames. means that you have to interact frame first before
        /// call this func.
        virtual const node& getEval() const;

    protected:
        virtual str _onRunSub(node& sub, const ucontainable& args);
        void _inFrame(const bicontainable& args) override {}
        void _outFrame() override {}
    };
}
