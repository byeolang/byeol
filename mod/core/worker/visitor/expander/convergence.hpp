#pragma once

#include "../../../ast/baseObj.hpp"
#include <functional>

namespace nm {

    class convergence: public instance {
        NM(CLASS(convergence, instance))

    public:
        convergence(baseObj& obj, baseFunc& func, std::function<nbool()> closure);

    public:
        /// converge type to real obj type.
        /// parser registered all obj in some obj.
        /// and expander is now visiting all object's sub nodes while interaction frames.
        /// in spite of that, getEval() of origin has been failed. which means,
        /// the origin, actually getExpr holding for a name, is refering variable not expanded yet.
        ///
        /// I need to replace 'getExpr(<name>)' type to proper real origin obj.
        /// if I don't do that, before on every try to access parameters or return type of a func,
        /// user must interacts proper scope of it to the frame. it's very tedious and redundant
        /// job.
        ///
        /// this requests of type convergence will be done when expand() done successfully.
        ///
        /// @return true if type has been converged successfully.
        nbool converge();

        std::function<nbool()> getClosure();

        baseFunc& getFunc();
        const baseFunc& getFunc() const NM_CONST_FUNC(getFunc());

        baseObj& getObj();
        const baseObj& getObj() const NM_CONST_FUNC(getObj());

    private:
        tstr<baseObj> _obj;
        tstr<baseFunc> _func;
        std::function<nbool()> _cl;
    };
}
