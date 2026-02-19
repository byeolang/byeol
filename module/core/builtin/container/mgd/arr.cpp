#include "core/builtin/container/mgd/arr.hpp"

#include <utility>

#include "core/bridge/cpp.hpp"
#include "core/worker/visitor/generalizer.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {

    BY(DEF_ME(arr), DEF_VISIT())

    namespace {
        static inline const std::string TYPENAME = "T";

        typedef tucontainable<node>::iter niter;
        typedef tbridge<niter> __superMgdIter;

        static const baseObj& _getOrigin() {
            static me inner;
            return inner;
        }

        class _nout mgdIter: public __superMgdIter {
            BY(CLASS(mgdIter, __superMgdIter))

        public:
            mgdIter(niter* real): super(real) {}

        public:
            using super::subs;

            scope& subs() override {
                static scope inner = tbridger<niter>::ctor()
                                         .ctor<niter>()
                                         .func("isEnd", &niter::isEnd)
                                         .func("next", &niter::next)
                                         .funcNonConst<node*>("get", &niter::get)
                                         .subs();

                return inner;
            }
        };

        class iterateFunc: public baseFunc {
            BY(ME(iterateFunc, baseFunc), CLONE(iterateFunc))

        public:
            const ntype& getType() const override {
                static mgdType inner("iterate", ttype<me>::get(), params(*new param("step", *new nInt())), false,
                    new mgdIter(nullptr));
                return inner;
            }

            const baseObj& getOrigin() const override { return _getOrigin(); }

            str eval(const args& a) override {
                const params& ps = getParams();
                WHEN(a.len() != ps.len()) .warn("a.len(%d) != ps.len(%d)", a.len(), ps.len()).ret(str());
                arr& meObj = a.getMe() TO(template cast<arr>()) OR.err("meObj as arr == null").ret(str());
                str eval = a[0].as(ps[0].getOrigin())
                               OR.err("evaluation of arg[%s] -> param[%s] has been failed", a[0], ps[0])
                                   .ret(str());

                nint step = *eval->cast<nint>();
                return new mgdIter(new niter(meObj.get()->iterate(step)));
            }
        };

        class getElemTypeFunc: public baseFunc {
            BY(ME(getElemTypeFunc, baseFunc), CLONE(getElemTypeFunc))

        public:
            getElemTypeFunc(): _type("getElemType", ttype<me>::get(), params(), false, new getExpr(TYPENAME)) {}

        public:
            const ntype& getType() const override { return _type; }

            const baseObj& getOrigin() const override { return _getOrigin(); }

            str eval(const args& a) override { return getType().getRet() TO(template as<node>()); }

        private:
            mgdType _type;
        };

        const static std::string paramName = "typeParam";
    } // namespace

    me::arr(): super(new narr()), _type("arr", ttype<me>::get(), params(*new param(paramName, *new obj()))) {}

    me::arr(const node& newType):
        super(new narr()), _type("arr", ttype<me>::get(), params(*new param(paramName, newType))) {}

    me::arr(const me& rhs): super(rhs), _type(rhs._type) {}

    node& me::operator[](nidx n) { return get()->operator[](n); }

    const ntype& me::getType() const { return _type; }

    scope& me::subs() {
        const auto& ps = getType().getParams();
        WHEN(ps.isEmpty()) .ret(dumScope::singleton());

        auto paramsOrg = ps[0].getOrigin().as<baseObj>() OR.ret(dumScope::singleton());
        const baseObj& paramOrg = paramsOrg->getOrigin();
        auto e = _cache.find(&paramOrg);
        WHEN(e != _cache.end()) .ret(*e->second.get());

        // this is first try to generalize type T:
        return _defGeneric(paramOrg);
    }

    ncnt me::len() const { return get()->len(); }

    nbool me::in(nidx n) const { return get()->in(n); }

    node* me::get(nidx n) { return get()->get(n); }

    /* TODO: can I remove this?
    node& me::get(std::function<nbool(const node&)> l) const {
        return this->get<node>(std::move(l));
    }

    narr me::getAll(std::function<nbool(const node&)> l) const {
        return this->getAll<node>(std::move(l));
    }
    */

    nbool me::set(const iter& at, const node& new1) {
        str ased = new1.asImpli(getType().getParams()[0].getOrigin());
        WHEN(!ased || ased->isSub<nVoid>()) .ret(false);

        return get()->set(at, *ased);
    }

    nbool me::set(nidx n, const node& new1) {
        str ased = new1.asImpli(getType().getParams()[0].getOrigin());
        WHEN(!ased || ased->isSub<nVoid>()) .ret(false);
        return get()->set(n, *ased);
    }

    nbool me::add(const iter& at, const node& new1) {
        str ased = new1.asImpli(getType().getParams()[0].getOrigin());
        WHEN(!ased || ased->isSub<nVoid>()) .ret(false);

        return get()->add(at, *ased);
    }

    nbool me::add(nidx n, const node& new1) {
        str ased = new1.asImpli(getType().getParams()[0].getOrigin());
        WHEN(!ased || ased->isSub<nVoid>()) .ret(false);

        return get()->add(n, *ased);
    }

    void me::add(const iter& here, const iter& from, const iter& to) { get()->add(here, from, to); }

    //  del:
    nbool me::del(nidx n) { return get()->del(n); }

    nbool me::del(const iter& it) { return get()->del(it); }

    nbool me::del(const iter& from, const iter& to) { return get()->del(from, to); }

    void me::rel() { get()->rel(); }

    const baseObj& me::getOrigin() const { return *this; }

    std::string me::asStr() const { return get()->asStr(); }

    me::iteration* me::_onMakeIteration(ncnt step, nbool isReversed) const {
        return get()->_onMakeIteration(step, isReversed);
    }

    namespace {
        class __copyCtor: public baseFunc {
            BY(ME(__copyCtor, baseFunc), CLONE(__copyCtor))

        public:
            __copyCtor(const baseObj& newType): _org(new arr(newType)) {}

        public:
            const ntype& getType() const override {
                static mgdType inner("copyctor", ttype<me>::get(), params(*new param("rhs", _org.get())), false,
                    _org.get());
                return inner;
            }

            const baseObj& getOrigin() const override { return *_org; }

            str eval(const args& a) override {
                node& src = a.getMe() OR.ret(str());
                return (node*) src.clone();
            }

        private:
            tstr<arr> _org;
        };
    }

    scope& me::_defGeneric(const baseObj& paramOrg) {
        scope* clone = (scope*) _getOriginScope().cloneDeep();
        _cache.insert({&paramOrg, clone}); // this avoids infinite loop.
        clone->add(ctor::CTOR_NAME,
            new tbridgeClosure<arr*, arr, tmarshaling>([&paramOrg](arr&) -> arr* { return new me(paramOrg); }));
        clone->add(ctor::CTOR_NAME, new __copyCtor(paramOrg));
        clone->add("getElemType", new getElemTypeFunc());

        BY_DI("|==============================================|");
        BY_DI("|--- generic: make arr<%s> generic class ---|", paramOrg);
        generalizer g;
        g.add(*new param(TYPENAME, paramOrg)).setTask(*this).setFlag(generalizer::INTERNAL).work();
        BY_DI("|============================|");

        return *clone;
    }

    scope& me::_getOriginScope() {
        static scope inner = tbridger<narr>::genericFunc("len", &narr::len)
                                 .genericFunc("rel", &narr::rel)
                                 .genericFunc<nbool, nidx>("del", &narr::del)
                                 .genericFunc<nbool, const node&>("add", &tucontainable<node>::add)
                                 .genericFunc<nbool, nidx, const node&>("add", &narr::add)
                                 .genericFunc<nbool, nidx, const node&>("set", &narr::set)
                                 .genericFuncNonConst<node*, nidx>("get", &narr::get)
                                 .genericFunc<nbool, const node&>("in", &narr::in)
                                 .func("iterate", new iterateFunc())
                                 .subs();

        return inner;
    }
} // namespace by
