#pragma once

#include "obj.hpp"
#include "../type/mgdType.hpp"

namespace nm {

    class src;
    class _nout origin : public obj {
        NM(ME(origin, obj),
           INIT_META(origin),
           CLONE(origin))

    public:
        friend class genericOrigin; // for _setOrigin(), _setType
        friend class slot; // for _onRunSub
        friend class preEvaluator;
        friend class exprMaker;
        friend class parser;

    public:
        explicit origin(const mgdType& newType);
        explicit origin(const mgdType& newType, scope& shares, scope& owns);
        explicit origin(const mgdType& newType, const origin& subpack, nbool isComplete);
        explicit origin(const me& rhs);

    public:
        me& operator=(const me& rhs);

    public:
        const ntype& getType() const override;
        const origin& getSubPack() const override;
        const src& getSrc() const override;
        clonable* cloneDeep() const override;
        const baseObj& getOrigin() const override;
        baseObj* make() const override;

    private:
        void _setType(const mgdType& new1);
        void _setSubPack(const obj& newSub);
        void _setSrc(const src& s) override;
        me& _assign(const me& rhs);

    private:
        mgdType _type; // TODO: memory leak
        tstr<origin> _subpack;
        tstr<src> _src;
    };
}
