#pragma once

#include "node.hpp"
#include "baseFunc.hpp"
#include "params.hpp"
#include "baseObj.hpp"
#include "tpriorities.inl"
#include "args.hpp"
#include "scope.hpp"
#include "../builtin/container/tbicontainable.inl"

namespace nm {

#define ME node
#define TEMPLATE template <typename T>

    template <typename T>
    class _nout tprioritiesBucket : public std::vector<tnarr<tprior<T>>> {
        typedef tnarr<tprior<T>> elem;
        typedef std::vector<elem> super;
    public:
        tnarr<tprior<T>>& operator[](nidx n) {
            while(n >= this->size())
                push_back(elem());
            return this->super::operator[](n);
        }
        const tnarr<tprior<T>>& operator[](nidx n) const NM_CONST_FUNC(tprioritiesBucket<T>, operator[](n))

    public:
        tpriorities<T> join() const {
            tpriorities<T> ret;
            for(int n=0; n < this->size(); n++) {
                for(const tprior<T>& elem : (*this)[n])
                    ret.add(new tprior<T>(*elem, priorType(n), elem.lv));
            }
            return ret;
        }
        using super::push_back;
        void push_back(const tprior<T>& elem) {
            (*this)[elem.type].add(elem);
        }
    };

    TEMPLATE
    T& ME::sub(std::function<nbool(const std::string&, const T&)> l) {
        return subs().get<T>(l);
    }

    TEMPLATE
    T& ME::sub() {
        return subs().get<T>();
    }

    TEMPLATE
    T& ME::sub(const std::string& name) {
#if NM_IS_DBG
        ncnt n = 0;
#endif
        return subs().get<T>([&](const std::string& key, const T& val) {
            NM_DI("sub: [%s/%s] %s --> %s", ++n, subs().len(), name, key);
            return key == name;
        });
    }

    TEMPLATE
    T& ME::sub(const std::string& name, const args& a) {
        if(nul(a))
            return sub<T>(name);

#if NM_IS_DBG
        ncnt n = 0;
#endif
        std::string argStr = a.toStr();
        return subs().get<T>([&](const std::string& key, const T& val) {
            priorType p = NO_MATCH;
            if(key == name) p = val.prioritize(a);

            NM_DI("sub: [%s/%s] %s(%s) --> %s = %s", ++n, subs().len(), name, argStr, key, getPriorTypeName(p));
            return p != NO_MATCH;
        });
    }

    TEMPLATE
    tnarr<T, strTactic> ME::subAll(std::function<nbool(const std::string&, const T&)> l) const {
        return subs().getAll<T>(l);
    }

    TEMPLATE
    tnarr<T, strTactic> ME::subAll() const {
        return subs().template getAll<T>();
    }

    TEMPLATE
    tpriorities<T> ME::subAll(const std::string& name) const {
        return subAll<T>(name, nulOf<args>());
    }

    TEMPLATE
    tpriorities<T> ME::subAll(const std::string& name, const args& a) const {
        // subs is arranged already to its scope:
        //  so if priorType of sub was same level, I need to keep the priorType of original container.

#if NM_IS_DBG
        ncnt n = 0;
        const nchar* myName = getType().getName().c_str();
        std::string argStr = !nul(a) ? "(" + a.toStr() + ")" : "";
#endif
        tprioritiesBucket<T> ps;
        const scope* e = &subs();
        ncnt lv = 0;
        while(e) {
            priorType p = NO_MATCH;
            e->getContainer().each<T>([&](const std::string& key, const T& val) {
                if(key != name) return true;

                p = !nul(a) ? val.prioritize(a) : EXACT_MATCH;
                if(p != NO_MATCH)
                    ps.push_back(*new tprior<T>(val, p, lv));
                const baseFunc& f = val.template cast<baseFunc>();
                std::string valArgs = !nul(f) ? "(" + f.getParams().toStr() + ")": "";
                NM_DI("subAll: [%s/%s] %s%s --> %s.%s%s = priority(type=%s, lv=%s)", n++,
                        subs().len(), name, argStr, myName, key, valArgs, getPriorTypeName(p), lv);
                return true;
            });

            e = &e->getNext();
            lv++;
        }
        return ps.join();
    }

#undef TEMPLATE
#undef ME
}
