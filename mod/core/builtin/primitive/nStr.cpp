#include "nStr.hpp"
#include "../../ast/defaultCtor.hpp"
#include "../../ast/defaultCopyCtor.hpp"
#include "nBool.hpp"
#include "nInt.hpp"
#include "nFlt.hpp"
#include "nChar.hpp"
#include "nByte.hpp"
#include "../../visitor/visitor.hpp"
#include "../container/mgd/seq.hpp"
#include "../../bridge/cpp.hpp"

namespace nm {

    NM(DEF_ME(nStr), DEF_VISIT())

    namespace {
        static me org;
        /*class lenFunc : public tApiBridge<nStr, nInt> {
            typedef tApiBridge<nStr, nInt> __super__;
            NM(CLASS(lenFunc, __super__))

        protected:
            str _onRun(nStr& cast, const args& a) const override {
                return new nInt(cast.len());
            }
        };

        class getFunc : public tApiBridge<nStr, nChar> {
            typedef tApiBridge<nStr, nStr> __super__;
            NM(CLASS(getFunc, __super__))

        protected:
            str _onRun(nStr& cast, const args& a) const override {
                if(a.len() != 1) return str();

                tstr<nInt> idx = a[0].as<nInt>();
                if(!idx)
                    return str();
                return new nChar(cast[idx->get()]);
            }

        public:
            const params& getParams() const override {
                static params inner;
                if(inner.len() <= 0) {
                    inner.add(new param("index", new nInt()));
                }

                return inner;
            }
        };

        class getSeqFunc : public tApiBridge<nStr, nStr> {
            typedef tApiBridge<nStr, nStr> __super__;
            NM(CLASS(getSeqFunc, __super__))

        protected:
            str _onRun(nStr& cast, const args& a) const override {
                if(a.len() != 1) return str();

                tstr<seq> s = a[0].as<seq>();
                if(!s) return str();

                nint start = (*s)[0].cast<nint>();
                nint end = (*s)[s->len()-1].cast<nint>() + 1;
                return cast.substr(start, end);
            }

        public:
            const params& getParams() const override {
                static params inner;
                if(inner.len() <= 0) {
                    inner.add(new param("index", new seq(nInt(0), nInt(1))));
                }

                return inner;
            }
        };*/

        typedef tucontainable<nChar>::iter niter;
        typedef tucontainable<nChar>::iteration iteration;
        typedef tcppBridge<niter> __superMgdIter;

        class bridgeIteration : public iteration {
            NM(CLASS(bridgeIteration, iteration))

        public:
            bridgeIteration(nStr& own, nidx n): _own(&own), _n(n) {}

            nbool isEnd() const override {
                return !_own->has(_n);
            }

            ncnt next(ncnt step) override {
                if(step <= 0) return 0;
                if(isEnd()) return 0;

                int len = _own->len(),
                    lastN = len - 1;
                int toLast = lastN - _n;

                _n += step;
                if(_n > lastN) {
                    _n = len;
                    step = toLast;
                }
                return step;
            }

            nChar& get() override {
                if(isEnd()) return nulOf<nChar>();
                _val.get() = (*_own)[_n];
                return _val;
            }

            using super::getContainer;
            tucontainable<nChar>& getContainer() override {
                return *_own;
            }

        protected:
            nbool _onSame(const typeProvidable& rhs) const override {
                const me& cast = (const me&) rhs;
                return isFrom(cast.getContainer()) && _n == cast._n;
            }

        private:
            nStr* _own;
            nChar _val;
            nidx _n;
        };

        class _nout mgdIter : public __superMgdIter {
            NM(CLASS(mgdIter, __superMgdIter))

        public:
            mgdIter(niter* real): super(real) {}

        public:
            using super::subs;
            scope& subs() override {
                static tbridger<niter>* inner = nullptr;
                if(nul(inner)) {
                    inner = new tbridger<niter>(new niter());
                    inner->func("isEnd", &niter::isEnd)
                          .func("next", &niter::next)
                          .funcNonConst<nChar&>("get", &niter::get);
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
                if(a.len() != ps.len()) return NM_W("a.len(%d) != ps.len(%d)", a.len(), ps.len()), str();
                nStr& me = a.getMe().cast<nStr>();
                if(nul(me)) return NM_E("me as nStr == null"), str();

                str eval = a[0].as(ps[0].getOrigin().as<node>());
                if(!eval)
                    return NM_E("evaluation of arg[%s] -> param[%s] has been failed", a[0], ps[0]), str();

                nint step = eval->cast<nint>();
                return new mgdIter(new niter(me.iterate(step)));
            }
        };

        class getElemType : public baseFunc {
            NM(CLASS(getElemType, baseFunc))

        public:
            str getRet() const override {
                static str inner(new nChar());
                return inner;
            }

            str run(const args& a) override {
                return getRet();
            }
        };
    }

    nbool me::nStrType::isImmutable() const { return true; }

    me::nStr() {}
    me::nStr(const nchar* val): super(std::string(val)) {}
    me::nStr(const std::string& val): super(val) {}

    scope& me::_onMakeSubs() const {
        static tbridger<me>* inner = nullptr;
        if(nul(inner)) {
            inner = new tbridger<me>(org);
            inner->func("len", &me::len)
                .func<nchar, nidx>("get", &me::get)
                .func("substr", &me::substr);
            inner->subs().add("iterate", new iterateFunc());
            inner->subs().add("getElemType", new getElemType());
        }

        return inner->subs();
    }

    me::iteration* me::_onMakeIteration(ncnt step) const {
        return new bridgeIteration((me&) *this, step);
    }

    const ases& me::nStrType::_getAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            struct asBool : public tas<nBool> {
                str as(const node& me, const type& to) const override {
                    const std::string& val = me.cast<std::string>();
                    try {
                        bool boolean = false;
                        if(val == "false")
                            boolean = false;
                        else if(val == "true")
                            boolean = "true";
                        else
                            boolean = stoi(val, nullptr, 0) == 0;
                        return str(new nBool(boolean));
                    } catch (std::invalid_argument& ex) {
                        return str();
                    }
                }
            };
            inner.add(new asBool());
            struct asFlt : public tas<nFlt> {
                str as(const node& me, const type& to) const override {
                    const std::string& val = me.cast<std::string>();
                    try {
                        nflt converted = stof(val);
                        return str(new nFlt(converted));
                    } catch (std::invalid_argument& ex) {
                        return str();
                    }
                }
            };
            inner.add(new asFlt());
            struct asInt: public tas<nInt> {
                str as(const node& me, const type& to) const override {
                    const std::string& val = me.cast<std::string>();
                    try {
                        nint converted = stoi(val, nullptr, 0);
                        return str(new nInt(converted));
                    } catch (std::invalid_argument& ex) {
                        return str();
                    }
                }
            };
            inner.add(new asInt());
            struct asByte : public tas<nByte> {
                str as(const node& me, const type& to) const override {
                    const std::string& val = me.cast<std::string>();
                    if(val.length() <= 0) return str();

                    return new nByte(val[0]);
                }
            };
            inner.add(new asByte());
            struct asChar : public tas<nChar> {
                str as(const node& me, const type& to) const override {
                    const std::string& val = me.cast<std::string>();
                    if(val.length() <= 0) return str();

                    return new nChar(val[0]);
                }
            };
            inner.add(new asChar());
        }

        return inner;
    }

    nchar me::operator[](nint n) const {
        return get()[n];
    }

    nint me::len() const {
        return get().length();
    }

    tstr<arithmeticObj> me::bitwiseNot() const {
        return *this;
        // TODO: throw error
    }

    const baseObj& me::getOrigin() const {
        return org;
    }

    /// @param end is exclusive.
    tstr<nStr> me::substr(nint start, nint end) {
        return tstr<nStr>(new nStr(get().substr(start, end - start)));
    }

    nbool me::has(nidx n) const {
        return 0 <= n && n < get().size();
    }

    nchar me::get(nidx n) const {
        return get()[n];
    }

    void me::add(const iter& here, const iter& from, const iter& to) {}

    nbool me::add(const iter& at, const nChar& new1) {
        return false; /* nStr is immutable*/
    }

    nbool me::set(const iter& at, const nChar& new1) {
        return false;
    }

    nbool me::del(const iter& at) {
        return false;
    }

    nbool me::del(const iter& from, const iter& end) {
        return false;
    }

    void me::rel() {}

    tstr<arithmeticObj> me::_add(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() + get()):
                new me(get() + rhs.as<me>()->get());
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

    nbool me::_eq(const arithmeticObj& rhs) const {
        return get() == rhs.asImpli<me>()->get();
    }

    nbool me::_ne(const arithmeticObj& rhs) const {
        return get() != rhs.asImpli<me>()->get();
    }

    nbool me::_gt(const arithmeticObj& rhs) const {
        return get() > rhs.asImpli<me>()->get();
    }

    nbool me::_lt(const arithmeticObj& rhs) const {
        return get() < rhs.asImpli<me>()->get();
    }

    nbool me::_ge(const arithmeticObj& rhs) const {
        return get() >= rhs.asImpli<me>()->get();
    }

    nbool me::_le(const arithmeticObj& rhs) const {
        return get() <= rhs.asImpli<me>()->get();
    }

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
}
