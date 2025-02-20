#include "nStr.hpp"

#include "../../bridge/cpp.hpp"
#include "../../worker/visitor/visitor.hpp"
#include "../container/mgd/seq.hpp"
#include "../../type/as/ases.hpp"
#include "nBool.hpp"
#include "nByte.hpp"
#include "nFlt.hpp"
#include "nInt.hpp"

namespace nm {

    NM(DEF_ME(nStr), DEF_VISIT())

    namespace {

        class getSeqFunc: public baseFunc {
            NM(ME(getSeqFunc, baseFunc), CLONE(getSeqFunc))

        public:
            str run(const args& a) override {
                if(a.len() != 1) return str();
                nStr& me = a.getMe().cast<nStr>() orRet str();
                tstr<seq> s = a[0].as<seq>() orRet str();

                nint start = (*s).get().getStart().get();
                nint end = (*s).get().getEnd().get();
                return me.substr(start, end);
            }

            const ntype& getType() const override {
                static mgdType inner("get", ttype<baseFunc>::get(),
                    params(*new param("range", new seq(nInt(0), nInt(1)))), false, *new nStr());
                return inner;
            }

            const baseObj& getOrigin() const override;
        };

        typedef tucontainable<nStr>::iter niter;
        typedef tucontainable<nStr>::iteration iteration;
        typedef tbridge<niter> __superMgdIter;

        class bridgeIteration: public iteration {
            NM(CLASS(bridgeIteration, iteration))

        public:
            bridgeIteration(nStr& own, nidx n, nbool isReversed):
                super(isReversed), _own(&own), _e(own.get().c_str(), isReversed) {
                _e.next(n);
            }

            nbool isEnd() const override { return _e.isEnd(); }

            void rel() override { _e.rel(); }

            ncnt stepForward(ncnt step) override { return _e.stepForward(step); }

            ncnt stepBackward(ncnt step) override { return _e.stepBackward(step); }

            nStr& get() override {
                _val = *_e;
                return _val;
            }

            using super::getContainer;

            tucontainable<nStr>& getContainer() override { return *_own; }

        protected:
            nbool _onSame(const typeProvidable& rhs) const override {
                const me& cast = (const me&) rhs;
                return isFrom(cast.getContainer()) && _e == cast._e;
            }

        private:
            nStr* _own;
            cpIter _e;
            nStr _val;
        };

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
                                         .funcNonConst<nStr&>("get", &niter::get)
                                         .subs();

                return inner;
            }
        };

        class iterateFunc: public baseFunc {
            NM(ME(iterateFunc, baseFunc), CLONE(iterateFunc))

        public:
            const ntype& getType() const override {
                static mgdType inner("iterate", ttype<me>::get(),
                    params(*new param("step", *new nInt())), false, *new mgdIter(nullptr));
                return inner;
            }

            const baseObj& getOrigin() const override;

            str run(const args& a) override {
                const params& ps = getParams();
                if(a.len() != ps.len())
                    return NM_W("a.len(%d) != ps.len(%d)", a.len(), ps.len()), str();
                nStr &me = a.getMe().cast<nStr>() orRet NM_E("me as nStr == null"), str();

                str eval = a[0].as(ps[0].getOrigin().as<node>()) orRet NM_E(
                    "evaluation of arg[%s] -> param[%s] has been failed", a[0], ps[0]),
                    str();

                nint step = eval->cast<nint>();
                return new mgdIter(new niter(me.iterate(step)));
            }
        };

        class getElemType: public baseFunc {
            NM(ME(getElemType, baseFunc), CLONE(getElemType))

        public:
            const ntype& getType() const override {
                static mgdType inner("getElemType", ttype<me>::get(), params(), false, *new nStr());
                return inner;
            }

            const baseObj& getOrigin() const override;

            str run(const args& a) override { return getRet(); }
        };

        static const baseObj& _defaultOrg() {
            static tbaseObjOrigin<me> org(tbridger<me>::ctor()
                    .ctor<nStr>()
                    .func("len", &me::len)
                    .func<nchar, nidx>("get", &me::get)
                    .func("substr", &me::substr)
                    .func("get", new getSeqFunc())
                    .func("iterate", new iterateFunc())
                    .func("getElemType", new getElemType())
                    .subs());
            return org;
        }

        const baseObj& getSeqFunc::getOrigin() const { return _defaultOrg(); }

        const baseObj& iterateFunc::getOrigin() const { return _defaultOrg(); }

        const baseObj& getElemType::getOrigin() const { return _defaultOrg(); }
    } // namespace

    nbool me::nStrType::isImmutable() const { return true; }

    const std::string& me::nStrType::getName() const {
        static std::string inner("str");
        return inner;
    }

    me::nStr(nchar character): super(std::string(1, character)) {}

    me::nStr(const nchar* val): super(std::string(val)) {}

    me::nStr(const std::string& val): super(val) {}

    me::iteration* me::_onMakeIteration(ncnt step, nbool isReversed) const {
        return new bridgeIteration((me&) *this, step, isReversed);
    }

    namespace {
        // define in unamed namespace in order to avoid symbol duplication.
        struct asBool: public tas<nBool> {
            NM(CLASS(asBool, tas<nBool>))

        public:
            str as(const node& me, const type& to) const override {
                const std::string& val = me.cast<std::string>();
                try {
                    bool boolean = false;
                    if(val == "false") boolean = false;
                    else if(val == "true") boolean = "true";
                    else boolean = stoi(val, nullptr, 0) == 0;
                    return str(new nBool(boolean));
                } catch(std::invalid_argument& ex) { return str(); }
            }
        };

        struct asFlt: public tas<nFlt> {
            NM(CLASS(asFlt, tas<nFlt>))

        public:
            str as(const node& me, const type& to) const override {
                const std::string& val = me.cast<std::string>();
                try {
                    nflt converted = stof(val);
                    return str(new nFlt(converted));
                } catch(std::invalid_argument& ex) { return str(); }
            }
        };

        struct asInt: public tas<nInt> {
            NM(CLASS(asInt, tas<nInt>))

        public:
            str as(const node& me, const type& to) const override {
                const std::string& val = me.cast<std::string>();
                try {
                    nint converted = stoi(val, nullptr, 0);
                    return str(new nInt(converted));
                } catch(std::invalid_argument& ex) { return str(); }
            }
        };

        struct asByte: public tas<nByte> {
            NM(CLASS(asByte, tas<nByte>))

        public:
            str as(const node& me, const type& to) const override {
                const std::string& val = me.cast<std::string>();
                if(val.length() <= 0) return str();

                return new nByte(val[0]);
            }
        };
    } // namespace

    const ases& me::nStrType::_getAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asBool());
            inner.add(new asFlt());
            inner.add(new asInt());
            inner.add(new asByte());
        }

        return inner;
    }

    nchar me::operator[](nint n) const { return get()[n]; }

    nint me::len() const { return cpIter(get().c_str()).remainLen(); }

    tstr<arithmeticObj> me::bitwiseNot() const {
        return *this;
        // TODO: throw error
    }

    const baseObj& me::getOrigin() const {
        const baseObj& supers = super::getOrigin();
        return &supers == this ? _defaultOrg() : supers;
    }

    /// @param end is exclusive.
    tstr<nStr> me::substr(nint start, nint end) {
        return tstr<nStr>(new nStr(get().substr(start, end - start)));
    }

    nbool me::in(nidx n) const { return 0 <= n && n < get().size(); }

    nchar me::get(nidx n) const { return get()[n]; }

    void me::add(const iter& here, const iter& from, const iter& to) {}

    nbool me::add(const iter& at, const nStr& new1) { return false; /* nStr is immutable*/ }

    nbool me::set(const iter& at, const nStr& new1) { return false; }

    nbool me::del(const iter& at) { return false; }

    nbool me::del(const iter& from, const iter& end) { return false; }

    void me::rel() {}

    tstr<arithmeticObj> me::_add(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() + get()) : new me(get() + rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_sub(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_mul(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_div(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_mod(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_bitwiseAnd(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_bitwiseXor(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_bitwiseOr(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_lshift(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_rshift(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    nbool me::_eq(const arithmeticObj& rhs) const { return get() == rhs.asImpli<me>()->get(); }

    nbool me::_ne(const arithmeticObj& rhs) const { return get() != rhs.asImpli<me>()->get(); }

    nbool me::_gt(const arithmeticObj& rhs) const { return get() > rhs.asImpli<me>()->get(); }

    nbool me::_lt(const arithmeticObj& rhs) const { return get() < rhs.asImpli<me>()->get(); }

    nbool me::_ge(const arithmeticObj& rhs) const { return get() >= rhs.asImpli<me>()->get(); }

    nbool me::_le(const arithmeticObj& rhs) const { return get() <= rhs.asImpli<me>()->get(); }

    nbool me::_logicalAnd(const arithmeticObj& rhs) const {
        // TODO: throw error
        return false;
    }

    nbool me::_logicalOr(const arithmeticObj& rhs) const {
        // TODO: throw error
        return false;
    }

    arithmeticObj& me::_mov(const arithmeticObj& rhs) {
        get() = rhs.asImpli<me>()->get();
        return *this;
    }
} // namespace nm
