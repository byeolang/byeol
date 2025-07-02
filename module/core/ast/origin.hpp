#pragma once

#include "core/type/mgdType.hpp"
#include "core/ast/obj.hpp"

namespace nm {

    /// origin class is represents user defined classes in managed codes.
    /// one important thing you must remember is, origin should be shadowed to 'obj' type.
    /// every interaction you can take with origin could be handled with obj class.
    /// and inherits something too origin class aren't allowed.
    ///
    /// this limitation affects to usage of binder too:
    /// simply, declaring binder with type parameter 'origin' is not allowed. use 'obj' type instead
    /// of.
    ///     e.g.
    ///         tstr<origin> a; // X, unexpected behavior may happen.
    ///         tstr<obj> a; // O
    class src;
    class runExpr;

    class _nout origin final: public obj {
        NM(ME(origin, obj), INIT_META(origin))

    public:
        friend class genericOrigin; // for _setOrigin(), _setType
        friend class slot;          // for _onRunSub
        friend class expander;
        friend class exprMaker;
        friend class parser;

    public:
        explicit origin(const mgdType& newType);
        explicit origin(const mgdType& newType, const node& subpack);
        explicit origin(const me& rhs);

    public:
        me& operator=(const me& rhs);

    public:
        using super::subs;
        scope& subs() override;

        state getState() const override;
        void setState(state new1) override;
        const ntype& getType() const override;
        const node& getSubPack() const override;
        const src& getSrc() const override;
        void onCloneDeep(const clonable& from) override;
        baseObj* make() const override;

        using super::getCallComplete;
        node* getCallComplete() override;

        void setCallComplete(const node& new1);
        void setCallComplete(const node* it) NM_SIDE_FUNC(setCallComplete);

        nbool isComplete() const override;
        clonable* clone() const override;
        clonable* cloneDeep() const override;

    private:
        void _setType(const mgdType& new1) override;
        void _setSubPack(const node& newSub);
        void _setSrc(const src& s) override;
        void _setModifier(const modifier& mod) override;
        me& _assign(const me& rhs);
        void _runCallComplete();

    private:
        mgdType _type; // TODO: memory leak
        str _subpack;
        tstr<src> _src;
        tstr<modifier> _mod;
        str _callComplete;
        state _state;
    };
} // namespace nm
