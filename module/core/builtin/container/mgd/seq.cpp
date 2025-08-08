#include "core/builtin/container/mgd/seq.hpp"

#include "core/worker/visitor/visitor.hpp"
#include "core/bridge/cpp.hpp"
#include "core/worker/visitor/visitInfo.hpp"

namespace by {

    NM(DEF_ME(seq), DEF_VISIT())

    namespace {
        static const baseObj& _getOrigin() {
            static seq inner(0, 0);
            return inner;
        }
    }

    me::seq(const nInt& start, const nInt& end): super(new nseq(start, end)) {}

    me::seq(const nInt& start, const nInt& end, const nInt& step):
        super(new nseq(start, end, step)) {}

    me::seq(): super(new nseq(0, 0)) {}

    nInt me::operator[](nidx n) { return get()[n]; }

    const ntype& me::getType() const {
        static mgdType inner("seq", ttype<me>::get(), params(*new param("typeParam", *new nInt())));
        return inner;
    }

    ncnt me::len() const { return get().len(); }

    nbool me::in(nidx n) const { return get().in(n); }

    //  get:
    nInt me::get(nidx n) { return get().get(n); }

    //  etc:
    void me::rel() { return get().rel(); }

    namespace {
        typedef tucontainable<nInt, nInt, nInt>::iter niter;
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
            NM(ME(iterateFunc, baseFunc), CLONE(iterateFunc))

        public:
            const ntype& getType() const override {
                static mgdType inner("iterate", ttype<me>::get(),
                    params(*new param("step", *new nInt())), false, new mgdIter(nullptr));
                return inner;
            }

            const baseObj& getOrigin() const override { return _getOrigin(); }

            str run(const args& a) override {
                const params& ps = getParams();
                WHEN(a.len() != ps.len())
                    .warn("a.len(%d) != ps.len(%d)", a.len(), ps.len())
                    .ret(str());

                seq& meObj = a.getMe() TO(template cast<seq>()) OR.err("meObj as arr == null").ret(str());
                str eval =
                    a[0].as(ps[0].getOrigin())
                        OR.err("evaluation of arg[%s] -> param[%s] has been failed", a[0], ps[0])
                            .ret(str());

                nint step = *eval->cast<nint>();
                static tucontainable<nInt, nInt, nInt>::iter (tucontainable<nInt, nInt, nInt>::*specifier)(ncnt)
                    const = &seq::iterate;
                return new mgdIter(new niter((meObj.get().*specifier)(step)));
            }
        };

        class getElemTypeFunc: public baseFunc {
            NM(ME(getElemTypeFunc, baseFunc), CLONE(getElemTypeFunc))

        public:
            const ntype& getType() const override {
                static mgdType inner("getElemType", ttype<me>::get(), params(), false, new nInt());
                return inner;
            }

            const baseObj& getOrigin() const override { return _getOrigin(); }

            str run(const args& a) override { return getRet(); }
        };
    } // namespace

    scope& me::subs() {
        static scope inner = tbridger<nseq>::ctor<nint, nint>()
                                 .ctor<nint, nint, nint>()
                                 .ctor<nseq>()
                                 .func("len", &nseq::len)
                                 .func<nInt, nint>("get", &nseq::get)
                                 .func<nbool, nint>("in", &nseq::in)
                                 .func("iterate", new iterateFunc())
                                 .func("getElemType", new getElemTypeFunc())
                                 .subs();

        return inner;
    }

    me::iteration* me::_onMakeIteration(ncnt step, nbool isReversed) const {
        return get()._onMakeIteration(step, isReversed);
    }

    nbool me::set(const iter& at, const nInt& new1) { return false; }

    nbool me::set(nidx n, const nInt& new1) { return false; }

    nbool me::add(const iter& at, const nInt& new1) { return false; }

    nbool me::add(nidx n, const nInt& new1) { return false; }

    void me::add(const iter& here, const iter& from, const iter& to) {}

    nbool me::del(nidx n) { return false; }

    nbool me::del(const iter& it) { return false; }

    nbool me::del(const iter& from, const iter& to) { return false; }
} // namespace by
