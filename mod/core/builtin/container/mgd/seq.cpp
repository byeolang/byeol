#include "seq.hpp"

#include "../../../bridge/cpp.hpp"
#include "../../../worker/visitor/visitInfo.hpp"
#include "../../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(seq), DEF_VISIT())

    me::seq(const nInt& start, const nInt& end): super(new nseq(start, end)) {}

    me::seq(const nInt& start, const nInt& end, const nInt& step):
        super(new nseq(start, end, step)) {}

    me::seq(): super(new nseq(0, 0)) {}

    nInt me::operator[](nidx n) { return get()[n]; }

    const ntype& me::getType() const {
        static ntype* inner = nullptr;
        if(nul(inner)) {
            inner = new ttype<seq>();
            inner->_getBeans().add(*new nInt());
        }

        return *inner;
    }

    ncnt me::len() const { return get().len(); }

    nbool me::in(nidx n) const { return get().in(n); }

    //  get:
    nInt me::get(nidx n) { return get().get(n); }

    //  etc:
    void me::rel() { return get().rel(); }

    namespace {
        typedef tucontainable<nInt, nInt>::iter niter;
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
                                         .func<nInt>("get", &niter::get)
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
                seq& meObj = a.getMe().cast<seq>();
                if(nul(meObj)) return NM_E("meObj as arr == null"), str();

                str eval = a[0].as(ps[0].getOrigin().as<node>());
                if(!eval)
                    return NM_E("evaluation of arg[%s] -> param[%s] has been failed", a[0], ps[0]),
                           str();

                nint step = eval->cast<nint>();
                static tucontainable<nInt, nInt>::iter (tucontainable<nInt, nInt>::*specifier)(ncnt)
                    const = &seq::iterate;
                return new mgdIter(new niter((meObj.get().*specifier)(step)));
            }
        };

        class getElemTypeFunc: public baseFunc {
            NM(CLASS(getElemTypeFunc, baseFunc))

        public:
            str getRet() const override {
                static str inner(new nInt());
                return inner;
            }

            str run(const args& a) override { return getRet(); }
        };
    } // namespace

    scope& me::subs() {
        static scope inner = tbridger<nseq>::ctor<nint, nint>()
                                 .ctor<nint, nint, nint>()
                                 .ctor<nseq>()
                                 .func("len", &nseq::len)
                                 .func<nInt, nint>("get", &nseq::get)
                                 .func("in", &nseq::in)
                                 .func("iterate", new iterateFunc())
                                 .func("getElemType", new getElemTypeFunc())
                                 .subs();

        return inner;
    }

    me::iteration* me::_onMakeIteration(ncnt step) const { return get()._onMakeIteration(step); }

    nbool me::set(const iter& at, const nInt& new1) { return false; }

    nbool me::set(nidx n, const nInt& new1) { return false; }

    nbool me::add(const iter& at, const nInt& new1) { return false; }

    nbool me::add(nidx n, const nInt& new1) { return false; }

    void me::add(const iter& here, const iter& from, const iter& to) {}

    nbool me::del(nidx n) { return false; }

    nbool me::del(const iter& it) { return false; }

    nbool me::del(const iter& from, const iter& to) { return false; }
} // namespace nm
