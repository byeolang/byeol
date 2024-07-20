#pragma once

#include "src/src.hpp"
#include "../frame/frameInteractable.hpp"

namespace nm {

    class baseFunc;
    typedef tnarr<baseFunc> funcs;
    class frame;
    class origin;
    class obj;

    /// baseObj handles frame injection event of all objects.
    class _nout baseObj : public node {
        NM(ADT(baseObj, node))
        friend class verifier;
        friend class obj;
        friend class defaultMakeCtor;
        friend class parser;
        friend class genericOrigin; // from genericOrigin::_makeGeneric()
        friend class exprMaker;

    protected:
        /// if you don't give any subs when construct an baseObj, _subs will be assigned to dummy array.
        /// instance on ctor of derived class.
        explicit baseObj() = default;

    public:
        using super::run;
        str run(const args& a) override;

        priorType prioritize(const args& a) const override;

        virtual const baseObj& getOrigin() const = 0;

        using super::inFrame;
        void inFrame(const bicontainable& args) override;
        using super::outFrame;
        void outFrame(const bicontainable& args) override;

        const src& getSrc() const override;

        virtual nbool isPreEvaluated() const;

        virtual const origin& getSubPack() const;

        virtual baseObj* make() const;

    protected:
        str _onRunSub(node& sub, const args& a) override;
        void _setSrc(const src& s) override;
        virtual void _inFrame(frame& fr, const bicontainable& args);

    public:
         inline static const std::string CTOR_NAME = "@ctor";
         inline static const std::string PRECTOR_NAME = "@preCtor";
    };
}
