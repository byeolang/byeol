#pragma once

#include "core/builtin/container/tbicontainable.inl"
#include "core/ast/args.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/ast/baseObj.hpp"
#include "core/ast/node.hpp"
#include "core/ast/params.hpp"
#include "core/ast/scope.hpp"
#include "core/ast/tpriorities.inl"

namespace by {

#define ME node
#define TEMPLATE template <typename T>

    template <typename T> class _nout tprioritiesBucket: public std::vector<tnarr<tprior<T>>> {
        typedef tnarr<tprior<T>> elem;
        typedef std::vector<elem> super;

    public:
        tprioritiesBucket(): _topPriority(priorType(priorType::NO_MATCH - 1)) {}

    public:
        elem& operator[](nidx n) {
            while(n >= this->size())
                this->super::push_back(elem());
            return this->super::operator[](n);
        }
        const elem& operator[](nidx n) const BY_CONST_FUNC(tprioritiesBucket<T>, operator[](n))

    public:
        tpriorities<T> join() const {
            tpriorities<T> ret;
            for(int n = 0; n < this->size(); n++) {
                const auto& matches = (*this)[n];
                if(matches.len() == 0) continue;
                tstr<tprior<T>> first;
                for(const tprior<T>& match: matches) {
                    tstr<tprior<T>> elem(new tprior<T>(*match, priorType(n), match.lv));
                    if(!first) {
                        first = elem;
                        ret._setPriorType(first->type);
                    }
                    if(!first->isSamePrecedence(*elem)) break;

                    ret.add(*elem->elem);
                }
                break;
            }
            return ret;
        }

        using super::push_back;

        void push_back(const tprior<T>& elem) {
            WHEN(elem.type > _topPriority) .ret(); // optimization.

            (*this)[elem.type].add(elem);
            _topPriority = _topPriority < elem.type ? _topPriority : elem.type;
        }

    private:
        priorType _topPriority;
    };

    TEMPLATE
    T* ME::sub(std::function<nbool(const std::string&, const T&)> l) { return subs().get<T>(l); }

    TEMPLATE
    T* ME::sub() { return subs().get<T>(); }

    TEMPLATE
    T* ME::sub(const std::string& name) {
#if BY_IS_DBG
        ncnt n = 0;
#endif
        return subs().get<T>([&](const std::string& key, const T& val) {
            BY_DI("sub: [%d/%d] %s --> %s@%s", ++n, subs().len(), name, key, (void*) &val);
            return key == name;
        });
    }

    TEMPLATE
    T* ME::sub(const std::string& name, const args& a) {
#if BY_IS_DBG
        ncnt n = 0;
#endif
        std::string argStr = a.toStr();
        return subs().get<T>([&](const std::string& key, const T& val) {
            priorType p = NO_MATCH;
            if(key == name) p = val.prioritize(a);

            BY_DI("sub: [%d/%d] %s(%s) --> %s@%s = %s", ++n, subs().len(), name, argStr, key, (void*) &val,
                getPriorTypeName(p));
            return p != NO_MATCH;
        });
    }

    TEMPLATE
    tnarr<T, strTactic> ME::subAll(std::function<nbool(const std::string&, const T&)> l) const {
        return subs().getAll<T>(l);
    }

    TEMPLATE
    tnarr<T, strTactic> ME::subAll() const { return subs().template getAll<T>(); }

    TEMPLATE
    tpriorities<T> ME::subAll(const std::string& name) const { return subAll<T>(name, nullptr); }

    TEMPLATE
    tpriorities<T> ME::subAll(const std::string& name, const args* a) const {
        // subs is arranged already to its scope:
        //  so if priorType of sub was same level, I need to keep the priorType of original
        //  container.

#if BY_IS_DBG
        ncnt n = 0;
        std::string myName = getType().createNameWithParams();
        std::string argStr = a ? "(" + a->toStr() + ")" : "";
#endif
        tprioritiesBucket<T> ps;
        const scope* e = &subs();
        ncnt lv = 0;
        while(e) {
            priorType p = NO_MATCH;
            e->getContainer().each<T>([&](const std::string& key, const T& val) {
                WHEN(key != name) .ret(true);

                p = a ? val.prioritize(*a) : EXACT_MATCH;
                if(p != NO_MATCH) ps.push_back(*new tprior<T>(val, p, lv));

                const baseFunc* f = val.template cast<baseFunc>();
                std::string valArgs = f ? "(" + f->getParams().toStr() + ")" : "";
                BY_DI("subAll: [%d/%d] %s%s --> %s.%s%s@%s = priority(type=%s, lv=%d)", n++, subs().len(), name, argStr,
                    myName, key, valArgs, (void*) &val, getPriorTypeName(p), lv);
                return true;
            });

            e = e->getNext();
            lv++;
        }
        return ps.join();
    }

    TEMPLATE
    tpriorities<T> ME::subAll(const std::string& name, const args& a) const { return subAll<T>(name, &a); }

#undef TEMPLATE
#undef ME
} // namespace by
