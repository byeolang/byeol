#include "arr.hpp"

#include <utility>
#include "../../../visitor/visitor.hpp"
#include "../../../visitor/generalizer.hpp"
#include "../../../bridge/cpp.hpp"

namespace nm {

    NM(DEF_ME(arr), DEF_VISIT())

    namespace {
        static inline const std::string TYPENAME = "T";

        typedef tucontainable<node>::iter niter;
        typedef tbridge<niter> __superMgdIter;

        class _nout mgdIter : public __superMgdIter {
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

        class iterateFunc : public baseFunc {
            NM(CLASS(iterateFunc, baseFunc))

        public:
            str getRet() const override {
                static str inner(new mgdIter(nullptr));
                return inner;
            }

            const params& getParams() const override {
                static params inner;
                if(inner.len() <= 0)
                    inner.add(new param("step", *new nInt()));

                return inner;
            }

            str run(const args& a) override {
                const params& ps = getParams();
                if(a.len() != ps.len()) return NM_W("a.len(%d) != ps.len(%d)", a.len(), ps.len()), str();
                arr& meObj = a.getMe().cast<arr>();
                if(nul(meObj)) return NM_E("meObj as arr == null"), str();

                str eval = a[0].as(ps[0].getOrigin().as<node>());
                if(!eval)
                    return NM_E("evaluation of arg[%s] -> param[%s] has been failed", a[0], ps[0]), str();

                nint step = eval->cast<nint>();
                return new mgdIter(new niter(meObj.get().iterate(step)));
            }
        };

        class getElemTypeFunc : public baseFunc {
            NM(CLASS(getElemTypeFunc, baseFunc))

        public:
            getElemTypeFunc(): _ret(new getExpr(TYPENAME)) {}

        public:
            str getRet() const override {
                return _ret;
            }

            str run(const args& a) override {
                return _ret ? _ret->as<node>() : *_ret;
            }

            nbool setRet(const node& newRet) override {
                return _ret.bind(newRet);
            }

        private:
            str _ret;
        };
    }

    me::arr(): super(new narr()) { _type.getBeans().add(*new obj()); }
    me::arr(const node& newType): super(new narr()) { _type.getBeans().add(newType); }
    me::arr(const me& rhs): super(rhs) { _type.getBeans().add(rhs._type.getBeans()[0]); }

    node& me::operator[](nidx n) {
        return get()[n];
    }

    const ntype& me::getType() const {
        return _type;
    }

    scope& me::subs() {
        static dumScope dummy;
        const auto& beans = getType().getBeans();
        if(beans.isEmpty()) return dummy;

        const type* key = &beans[0].getType();
        auto e = _cache.find(key);
        if(e != _cache.end())
            return e->second.get();

        // this is first try to generalize type T:
        return _defGeneric(key);
    }

    ncnt me::len() const {
        return get().len();
    }

    nbool me::has(nidx n) const {
        return get().has(n);
    }

    node& me::get(nidx n) {
        return get()[n];
    }

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

    void me::add(const iter& here, const iter& from, const iter& to) {
        get().add(here, from, to);
    }

    //  del:
    nbool me::del(nidx n) {
        return get().del(n);
    }

    nbool me::del(const iter& it) {
        return get().del(it);
    }

    nbool me::del(const iter& from, const iter& to) {
        return get().del(from, to);
    }

    //  etc:
    clonable* me::cloneDeep() const {
        me* ret = (me*) clone();
        ret->rel();
        tstr<tnarr<node>> cloned((tnarr<node>*) get().cloneDeep());
        ret->get().add(*cloned);
        return ret;
    }

    void me::rel() {
       get().rel();
    }

    const baseObj& me::getOrigin() const {
        if(!_org)
            _org.bind(new me(getType().getBeans()[0]));
        return *_org;
    }

    std::string me::asStr() const {
        return get().asStr();
    }

    me::iteration* me::_onMakeIteration(ncnt step) const {
        return get()._onMakeIteration(step);
    }

    scope& me::_defGeneric(const type* key) {
        tstr<scope> clone((scope*)_getOriginScope().cloneDeep());
        clone->add("getElemType", new getElemTypeFunc());

        _cache.insert({key, clone}); // this avoids infinite loop.

        NM_DI("|==============================================|");
        NM_DI("|--- generic: make arr<%s> generic class ---|", key->getName());
        generalizer g;
        g.add(*new param(TYPENAME, getType().getBeans()[0]))
         .setTask(*this).setFlag(generalizer::INTERNAL).work();
        NM_DI("|============================|");

        return *clone;
    }

    /*
        key unmatch일 경우:
        0. param::deepClone()시 origin도 복제가 되어야 한다. ===> 이걸 위한 TC를 만들고 테스트.
            params.deepClone() 후, 원본.origin의 일부 속성을 변경. 그후, 복제된 params는 변경된 객체를 가리키는지 체크.
        1. 맨 먼저 origin 객체에서 복제한 새로운 origin 객체를 하나 만듬. 이 key에 대한 origin.
        2. _getOriginScope()의 동작은 기존 유지. 이때 기본생성자는 bridge하지 않아야 함.
        3. 갸져온 _getOriginScope()를 deepClone() 하여 새로운 untemplated origin에 넣음.
        4. _untemplated에 기본생성자에 대한 tbridgeClosure() 도 넣음 (bridger사용 X)
            넣을때 tstr<untemplated> 을 bridgeClosure() 안에 넣을 건데, 이 untemplated가 사라지지 않도록 처리 & 사전 검증 필요.
            예시:
                tstr<baseObj> untemplated = deepClone()....;
                cache.insert(..., untemplated); // 이제 tstr 자체는 사라져도, untemplated 자체는 사라지지 않음.
                untemplated->subs().add(baseObj::PRECTOR_NAME, [&untemplated](arr&) -> arr& {
                    return new arr(*untemplated);
                })

            이처럼 tstr을 lambda에서 catch 하는 경우, reference count는 어떻게 되는지, googling해보고, TC도 만들어야 한다.
        5. 이 untemplated를 generalizer를 돌려서 최종 완성시킴. --> templated.
        4. 이제 이 복제한 templated를 cache에 넣음 (cache는 scope에 대한 cache가 아니라 obj에 대한 cache.)
    */

    scope& me::_getOriginScope() {
        static scope inner = tbridger<narr>::closure<arr&, arr>(baseObj::PRECTOR_NAME, [](arr&) -> arr& {
            return *new arr();
        }).ctor<arr, arr>()
            .genericFunc("len", &narr::len)
            .genericFunc("rel", &narr::rel)
            .genericFunc<nbool, nidx>("del", &narr::del)
            .genericFunc<nbool, const node&>("add", &tucontainable<node>::add)
            .genericFunc<nbool, nidx, const node&>("add", &narr::add)
            .genericFunc<nbool, nidx, const node&>("set", &narr::set)
            .genericFuncNonConst<node&, nidx>("get", &narr::get)
            .genericFunc<nbool, const node&>("has", &narr::has)
            .func("iterate", new iterateFunc())
            .subs();

        return inner;
    }
}
