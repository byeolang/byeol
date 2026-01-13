/// @file
#pragma once

#include "core/ast/baseObj.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/frame/frameInteract.hpp"
#include "core/worker/visitor/expander/convergence/baseConvergence.hpp"

namespace by {

    template <typename T> struct ConvergenceClosure {
        typedef std::function<nbool(const T&)> is;
    };

    template <> struct ConvergenceClosure<void> {
        typedef std::function<nbool()> is;
    };

    template <typename T> class tbaseConvergence: public baseConvergence {
        BY(ADT(tbaseConvergence, baseConvergence))

    protected:
        using onConverge = typename ConvergenceClosure<T>::is;

    public:
        tbaseConvergence(baseObj& obj, baseFunc& func, onConverge closure):
            _obj(obj), _func(func), _onConverge(closure) {}

    public:
        /**
         *  converge type to real obj type.
         *  parser registered all obj in some obj.
         *  and expander is now visiting all object's sub nodes while interaction frames.
         *  in spite of that, infer()origin has been failed. which means,
         *  the origin, actually getExpr holding for a name, is refering variable not expanded yet.
         *
         *  I need to replace 'getExpr(<name>)' type to proper real origin obj.
         *  if I don't do that, before on every try to access parameters or return type of a func,
         *  user must interacts proper scope of it to the frame. it's very tedious and redundant
         *  job.
         *
         *  this requests of type convergence will be done when expand() done successfully.
         *
         *  @return true if type has been converged successfully.
         */
        nbool converge() override {
            WHEN(!_obj) .ret(false);
            baseFunc& fun = _func OR.ret(false);

            frameInteract objScope(*_obj);
            frameInteract funScope(fun);
            return convergeWithoutFrame();
        }

        baseFunc& getFunc() { return *_func; }

        const baseFunc& getFunc() const BY_CONST_FUNC(getFunc());

        baseObj& getObj() { return *_obj; }

        const baseObj& getObj() const BY_CONST_FUNC(getObj());

    protected:
        onConverge _getClosure() { return _onConverge; }

    private:
        tstr<baseObj> _obj;
        tstr<baseFunc> _func;
        onConverge _onConverge;
    };
} // namespace by
