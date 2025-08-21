/// @file
#pragma once

#include "core/ast/baseFunc.hpp"
#include "core/ast/params.hpp"
#include "core/ast/args.hpp"

namespace by {

    template <typename T> class _nout printFunc: public baseFunc {
        BY(ME(printFunc, baseFunc), CLONE(printFunc))

    public:
        const ntype& getType() const override {
            static mgdType inner("print", ttype<me>::get(), params(*new param("msg", new T())), false, new T());
            return inner;
        }

        const baseObj& getOrigin() const override {
            static obj inner(scope{
                {"print", new me()}
            });
            return inner;
        }

        str run(const args& a) override {
            const params& ps = getParams();
            WHEN(a.len() != ps.len())
                .err("length of args(%d) and typs(%d) doesn't match.", a.len(), ps.len()).ret(nullptr);

            const node& org = ps[0].getOrigin();
            tstr<T> evaluated = a[0].asImpli(*org.as<T>());
            WHEN(!evaluated) .err("evaluation of arg[%s] -> param[%s] has been failed.", a[0], org).ret(str());

            std::cout << evaluated->get();
            return evaluated;
        }
    };
}
