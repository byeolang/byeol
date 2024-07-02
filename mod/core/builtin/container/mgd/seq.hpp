#pragma once

#include "../../../ast.hpp"
#include "../native/nseq.hpp"
#include "../../../bridge/cpp/tcppBridge.hpp"

namespace nm {

    // another f**king annoying MSVC Hack:
    //  to avoid C2901 error, I need to declare sort of dllexport(import) things at here.
    //  spended plenty of hours again to find out the reason. thank you so much.
    typedef class _nout tcppBridge<nseq> __seqSuperClass;

    class _nout seq : public __seqSuperClass, public tucontainable<nInt, nInt>, tarrayable<nInt, nInt> {
        // seq uses wrapType:
        //  wrapType contains beanType as 'const type*' instance variable. so user should be
        //  careful when calling ttype<arr>. because it will also return wrapType instance
        //  as a consequences, but it won't contain beanType.
        //
        //  the most appropriate getter for wrapType of arr is to call getType() of instance
        //  to arr.
        NM(ME(seq, __seqSuperClass),
             INIT_META(seq),
             CLONE(seq),
             VISIT())

    public:
        typedef ntype metaType; // for ttype<T>
        typedef typename tucontainable<nInt, nInt>::iter iter;
        typedef typename tucontainable<nInt, nInt>::iteration iteration;

    public:
        seq(nInt start, nInt end);
        seq(nInt start, nInt end, nInt step);

    public:
        using tarrayable<nInt, nInt>::operator[];
        nInt operator[](nidx n) override;

    public:
        const ntype& getType() const override;

        using super::subs;
        scope& subs() override;

        ncnt len() const override;

        nbool has(nidx n) const override;

        //  get:
        using tarrayable<nInt, nInt>::get;
        using super::get;
        nInt get(nidx n) override;

        //  etc:
        void rel() override;

    protected:
        iteration* _onMakeIteration(ncnt step) const override;

    private:
        // these method has been prohibited.
        //  set:
        using tucontainable<nInt, nInt>::set;
        using tarrayable<nInt, nInt>::set;
        nbool set(const iter& at, const nInt& new1) override;
        nbool set(nidx n, const nInt& new1) override;
        //  add:
        using tucontainable<nInt, nInt>::add;
        using tarrayable<nInt, nInt>::add;
        nbool add(const iter& at, const nInt& new1) override;
        nbool add(nidx n, const nInt& new1) override;
        void add(const iter& here, const iter& from, const iter& to) override;
        //  del:
        using tucontainable<nInt, nInt>::del;
        nbool del(nidx n) override;
        nbool del(const iter& it) override;
        nbool del(const iter& from, const iter& to) override;
    };
}
