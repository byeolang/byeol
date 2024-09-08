#include "arr.hpp"

#include <utility>

#include "../../../bridge/cpp.hpp"
#include "../../../worker/visitor/generalizer.hpp"
#include "../../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(arr), DEF_VISIT())

    namespace {
        static inline const std::string TYPENAME = "T";

        typedef tucontainable<node>::iter niter;
        typedef tbridge<niter> __superMgdIter;

        class _nout mgdIter: public __superMgdIter {
            NM(CLASS(mgdIter, __superMgdIter))

        public:
            mgdIter(niter* real): super(real) {}

        public:
            using super::subs;

            scope& subs() override {
                static scope inner = tbridger<niter>::ctor()
                                         .ctor<niter>()
                                         .func("isEnd", &niter::isEnd)
                                         .func("next", &niter::next)
                                         .funcNonConst<node&>("get", &niter::get)
                                         .subs();

                return inner;
            }
        };

        class iterateFunc: public baseFunc {
            NM(CLASS(iterateFunc, baseFunc))

        public:
            str getRet() const override {
                static str inner(new mgdIter(nullptr));
                return inner;
            }

            const params& getParams() const override {
                static params inner;
                if(inner.len() <= 0) inner.add(new param("step", *new nInt()));

                return inner;
            }

            str run(const args& a) override {
                const params& ps = getParams();
                if(a.len() != ps.len())
                    return NM_W("a.len(%d) != ps.len(%d)", a.len(), ps.len()), str();
                arr& meObj = a.getMe().cast<arr>();
                if(nul(meObj)) return NM_E("meObj as arr == null"), str();

                str eval = a[0].as(ps[0].getOrigin().as<node>());
                if(!eval)
                    return NM_E("evaluation of arg[%s] -> param[%s] has been failed", a[0], ps[0]),
                           str();

                nint step = eval->cast<nint>();
                return new mgdIter(new niter(meObj.get().iterate(step)));
            }
        };

        class getElemTypeFunc: public baseFunc {
            NM(CLASS(getElemTypeFunc, baseFunc))

        public:
            getElemTypeFunc(): _ret(new getExpr(TYPENAME)) {}

        public:
            str getRet() const override { return _ret; }

            str run(const args& a) override { return _ret ? _ret->as<node>() : *_ret; }

            nbool setRet(const node& newRet) override { return _ret.bind(newRet); }

        private:
            str _ret;
        };
    } // namespace

    me::arr(): super(new narr()) { _type._getBeans().add(*new obj()); }

    me::arr(const node& newType): super(new narr()) { _type._getBeans().add(newType); }

    me::arr(const me& rhs): super(rhs) { _type._getBeans().add(rhs._type.getBeans()[0]); }

    node& me::operator[](nidx n) { return get()[n]; }

    const ntype& me::getType() const { return _type; }

    scope& me::subs() {
        static dumScope dummy;
        const auto& beans = getType().getBeans();
        if(beans.isEmpty()) return dummy;

        const node& paramType = beans[0];
        auto e = _cache.find(&paramType.getType());
        if(e != _cache.end()) return e->second.get();

        // this is first try to generalize type T:
        return _defGeneric(paramType);
    }

    ncnt me::len() const { return get().len(); }

    nbool me::in(nidx n) const { return get().in(n); }

    node& me::get(nidx n) { return get()[n]; }

    node& me::get(std::function<nbool(const node&)> l) const {
        return this->get<node>(std::move(l));
    }

    narr me::getAll(std::function<nbool(const node&)> l) const {
        return this->getAll<node>(std::move(l));
    }

    nbool me::set(const iter& at, const node& new1) {
        str ased = safeGet(new1, asImpli(getType().getBeans()[0]));
        if(!ased || ased->isSub<nVoid>()) return false;

        return get().set(at, *ased);
    }

    nbool me::set(nidx n, const node& new1) {
        str ased = safeGet(new1, asImpli(getType().getBeans()[0]));
        if(!ased || ased->isSub<nVoid>()) return false;
        return get().set(n, *ased);
    }

    nbool me::add(const iter& at, const node& new1) {
        str ased = safeGet(new1, asImpli(getType().getBeans()[0]));
        if(!ased || ased->isSub<nVoid>()) return false;

        return get().add(at, *ased);
    }

    nbool me::add(nidx n, const node& new1) {
        str ased = safeGet(new1, asImpli(getType().getBeans()[0]));
        if(!ased || ased->isSub<nVoid>()) return false;

        return get().add(n, *ased);
    }

    void me::add(const iter& here, const iter& from, const iter& to) { get().add(here, from, to); }

    //  del:
    nbool me::del(nidx n) { return get().del(n); }

    nbool me::del(const iter& it) { return get().del(it); }

    nbool me::del(const iter& from, const iter& to) { return get().del(from, to); }

    void me::rel() { get().rel(); }

    const baseObj& me::getOrigin() const { return *this; }

    std::string me::asStr() const { return get().asStr(); }

    me::iteration* me::_onMakeIteration(ncnt step) const { return get()._onMakeIteration(step); }

    namespace {
        class __copyCtor: public baseFunc {
            NM(CLASS(__copyCtor, baseFunc))

        public:
            __copyCtor(const node& newType): _org(new arr(newType)) {}

        public:
            str getRet() const override { return *_org; }

            const params& getParams() const override {
                static params inner{*new param("src", *_org)};
                return inner;
            }

            str run(const args& a) override {
                node& src = a.getMe();
                if(nul(src)) return str();

                return (node*) src.clone();
            }

        private:
            tstr<arr> _org;
        };
    }

    scope& me::_defGeneric(const node& paramType) {
        scope* clone = (scope*) _getOriginScope().cloneDeep();
        _cache.insert({&paramType.getType(), clone}); // this avoids infinite loop.
        clone->add(baseObj::CTOR_NAME,
            new tbridgeClosure<arr*, arr, tmarshaling>(
                [&paramType](arr&) -> arr* { return new me(paramType); }));
        clone->add(baseObj::CTOR_NAME, new __copyCtor(paramType));
        clone->add("getElemType", new getElemTypeFunc());

        NM_DI("|==============================================|");
        NM_DI("|--- generic: make arr<%s> generic class ---|", paramType);
        generalizer g;
        g.add(*new param(TYPENAME, paramType)).setTask(*this).setFlag(generalizer::INTERNAL).work();
        NM_DI("|============================|");

        return *clone;
    }

    scope& me::_getOriginScope() {
        static scope inner = tbridger<narr>::genericFunc("len", &narr::len)
                                 .genericFunc("rel", &narr::rel)
                                 .genericFunc<nbool, nidx>("del", &narr::del)
                                 .genericFunc<nbool, const node&>("add", &tucontainable<node>::add)
                                 .genericFunc<nbool, nidx, const node&>("add", &narr::add)
                                 .genericFunc<nbool, nidx, const node&>("set", &narr::set)
                                 .genericFuncNonConst<node&, nidx>("get", &narr::get)
                                 .genericFunc<nbool, const node&>("in", &narr::in)
                                 .func("iterate", new iterateFunc())
                                 .subs();

        return inner;
    }
} // namespace nm
