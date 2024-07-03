#include "seq.hpp"
#include "../../../visitor/visitInfo.hpp"
#include "../../../visitor/visitor.hpp"
#include "../../../bridge/cpp.hpp"

namespace nm {

    NM(DEF_ME(seq), DEF_VISIT())

    me::seq(nInt start, nInt end): super(new nseq(start, end)) {}
    me::seq(nInt start, nInt end, nInt step): super(new nseq(start, end, step)) {}

    nInt me::operator[](nidx n) {
        return get()[n];
    }

    const ntype& me::getType() const {
        static ntype* inner = nullptr;
        if(nul(inner)) {
            inner = new ttype<seq>();
            inner->getBeans().add(*new nInt());
        }

        return *inner;
    }

    ncnt me::len() const {
        return get().len();
    }

    nbool me::has(nidx n) const {
        return get().has(n);
    }

    //  get:
    nInt me::get(nidx n) {
        return get().get(n);
    }

    //  etc:
    void me::rel() {
        return get().rel();
    }

    namespace {
        typedef tucontainable<nInt, nInt>::iter niter;
        typedef tcppBridge<niter> __superMgdIter;
        class _nout mgdIter : public __superMgdIter {
            NM(CLASS(mgdIter, __superMgdIter))

        public:
            mgdIter(niter* real): super(real) {}

        public:
            using super::subs;
            scope& subs() override {
                static super* inner = nullptr;
                if(nul(inner)) {
                    inner = new super();
                    inner->func("isEnd", &niter::isEnd)
                          .func("next", &niter::next)
                          .func<nInt>("get", &niter::get);
                }

                return inner->subs();
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
                if(a.len() != ps.len()) return NM_W("a.len(%s) != ps.len(%s)", a.len(), ps.len()), str();
                seq& meObj = a.getMe().cast<seq>();
                if(nul(meObj)) return NM_E("meObj as arr == null"), str();

                str eval = a[0].as(ps[0].getOrigin().as<node>());
                if(!eval)
                    return NM_E("evaluation of arg[%s] -> param[%s] has been failed", a[0], ps[0]), str();

                nint step = eval->cast<nint>();
                static tucontainable<nInt, nInt>::iter (tucontainable<nInt, nInt>::*specifier)(ncnt) const = &seq::iterate;
                return new mgdIter(new niter((meObj.get().*specifier)(step)));
            }
        };

        class getElemTypeFunc : public baseFunc {
            NM(CLASS(getElemTypeFunc, baseFunc))

        public:
            str getRet() const override {
                static str inner(new nInt());
                return inner;
            }

            str run(const args& a) override {
                return getRet();
            }
        };
    }

    scope& me::subs() {
        static super* inner;
        if(nul(inner)) {
            inner = new super();
            inner->func("len", &nseq::len)
                  .func<nInt, nint>("get", &nseq::get)
                  .func("has", &nseq::has);
            inner->subs().add("iterate", new iterateFunc());
            inner->subs().add("getElemType", new getElemTypeFunc());
        }

        return inner->subs();
    }

    me::iteration* me::_onMakeIteration(ncnt step) const {
        return get()._onMakeIteration(step);
    }

    nbool me::set(const iter& at, const nInt& new1) {
        return false;
    }

    nbool me::set(nidx n, const nInt& new1) {
        return false;
    }

    nbool me::add(const iter& at, const nInt& new1) {
        return false;
    }

    nbool me::add(nidx n, const nInt& new1) {
        return false;
    }

    void me::add(const iter& here, const iter& from, const iter& to) {}

    nbool me::del(nidx n) {
        return false;
    }

    nbool me::del(const iter& it) {
        return false;
    }

    nbool me::del(const iter& from, const iter& to) {
        return false;
    }
}
