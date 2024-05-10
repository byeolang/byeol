#pragma once

#include "src/src.hpp"
#include "../frame/frameInteractable.hpp"

namespace namu {

    class baseFunc;
    typedef tnarr<baseFunc> funcs;

    /// baseObj handles frame injection event of all objects.
    class _nout baseObj : public node {
        NAMU(ADT(baseObj, node))
        friend class verifier;
        friend class obj;
        friend class defaultMakeCtor;
        friend class parser;

    protected:
        /// if you don't give any subs when construct an baseObj, _subs will be assigned to dummy array.
        /// instance on ctor of derived class.
        explicit baseObj();

    public:
        using super::run;
        str run(const args& a) override;

        priority prioritize(const args& a) const override;

        virtual const baseObj& getOrigin() const = 0;

        using super::inFrame;
        void inFrame(const bicontainable& args) override;
        void outFrame() override;

        str getEval() const override;

    protected:
        str _onRunSub(node& sub, const args& a) override;

    public:
         inline static const string CTOR_NAME = "@ctor";
         inline static const string PRECTOR_NAME = "@preCtor";
    };
}
