#pragma once

#include "core/ast.hpp"
#include "core/bridge/cpp/tbridge.hpp"
#include "core/builtin/container/native/nseq.hpp"

namespace nm {

    typedef class _nout tbridge<nseq> __seqSuperClass;

    class _nout seq: public __seqSuperClass,
                     public tucontainable<nInt, nInt, nInt>,
                     tarrayable<nInt, nInt, nInt> {
        // seq uses wrapType:
        //  wrapType contains beanType as 'const type*' instance variable. so user should be
        //  careful when calling ttype<arr>. because it will also return wrapType instance
        //  as a consequences, but it won't contain beanType.
        //
        //  the most appropriate getter for wrapType of arr is to call getType() of instance
        //  to arr.
        NM(ME(seq, __seqSuperClass), INIT_META(seq), CLONE(seq), VISIT())

    public:
        typedef ntype metaType; // for ttype<T>
        typedef typename tucontainable<nInt, nInt, nInt>::iter iter;
        typedef typename tucontainable<nInt, nInt, nInt>::iteration iteration;
        template <typename T, nbool> friend struct tmarshaling;

    public:
        seq(const nInt& start, const nInt& end);
        seq(const nInt& start, const nInt& end, const nInt& step);

    private:
        // @hidden this's only available to marshaling.
        seq();

    public:
        using tarrayable<nInt, nInt, nInt>::operator[];
        nInt operator[](nidx n) override;

    public:
        const ntype& getType() const override;

        using super::subs;
        scope& subs() override;

        ncnt len() const override;

        nbool in(nidx n) const override;

        //  get:
        using tarrayable<nInt, nInt, nInt>::get;
        using super::get;
        nInt get(nidx n) override;

        //  etc:
        void rel() override;

    protected:
        iteration* _onMakeIteration(ncnt step, nbool isReversed) const override;

    private:
        // these method has been prohibited.
        //  set:
        using tucontainable<nInt, nInt, nInt>::set;
        using tarrayable<nInt, nInt, nInt>::set;
        nbool set(const iter& at, const nInt& new1) override;
        nbool set(nidx n, const nInt& new1) override;
        //  add:
        using tucontainable<nInt, nInt, nInt>::add;
        using tarrayable<nInt, nInt, nInt>::add;
        nbool add(const iter& at, const nInt& new1) override;
        nbool add(nidx n, const nInt& new1) override;
        void add(const iter& here, const iter& from, const iter& to) override;
        //  del:
        using tucontainable<nInt, nInt, nInt>::del;
        nbool del(nidx n) override;
        nbool del(const iter& it) override;
        nbool del(const iter& from, const iter& to) override;
    };
} // namespace nm
