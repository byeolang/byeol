#pragma once

#include "../type/mgdType.hpp"
#include "obj.hpp"

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
        NM(ME(origin, obj), INIT_META(origin), CLONE(origin))

    public:
        friend class genericOrigin; // for _setOrigin(), _setType
        friend class slot;          // for _onRunSub
        friend class expander;
        friend class exprMaker;
        friend class parser;

    public:
        explicit origin(const mgdType& newType);
        explicit origin(const mgdType& newType, scope& shares, scope& owns);
        explicit origin(const mgdType& newType, const obj& subpack);
        explicit origin(const me& rhs);

    public:
        me& operator=(const me& rhs);

    public:
        state getState() const override;
        void setState(state new1) override;
        const ntype& getType() const override;
        const obj& getSubPack() const override;
        const src& getSrc() const override;
        clonable* cloneDeep() const override;
        const baseObj& getOrigin() const override;
        baseObj* make() const override;
        const runExpr& getCallComplete() const;
        void setCallComplete(const runExpr& new1);
        nbool isComplete() const override;

    private:
        void _setType(const mgdType& new1) override;
        void _setSubPack(const obj& newSub);
        void _setSrc(const src& s) override;
        me& _assign(const me& rhs);

    private:
        mgdType _type; // TODO: memory leak
        tstr<obj> _subpack;
        tstr<src> _src;
        tstr<runExpr> _callComplete;
        state _state;
    };
} // namespace nm
