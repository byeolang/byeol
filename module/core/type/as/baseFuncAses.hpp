#pragma once

#include "core/builtin/primitive/nStr.hpp"
#include "core/ast/closure.hpp"
#include "core/type/as/tas.hpp"

namespace by {

    struct _nout baseFuncAsStr : public tas<nStr, baseFunc> {
        typedef tas<nStr, baseFunc> __super12;
        BY(CLASS(baseFuncAsStr, __super12))

    protected:
        str _onAs(const baseFunc& me, const type& to) const override;
    };

    /*struct _nout baseFuncAsClosure : public tas<closure, baseFunc> {
        typedef tas<closure, baseFunc> __super24;
        BY(CLASS(baseFuncAsClosure, __super24))

    public:
        nbool is(const type& from, const type& to) const override;

    protected:
        str _onAs(const baseFunc& me, const type& to) const override;
    };*/

    struct _nout baseFuncAsBaseFunc : public tas<baseFunc, baseFunc> {
        typedef tas<baseFunc, baseFunc> __super25;
        BY(CLASS(baseFuncAsBaseFunc, __super25))

    public:
        nbool is(const type& from, const type& to) const override;

    protected:
        str _onAs(const baseFunc& me, const type& to) const override;
    };
}
