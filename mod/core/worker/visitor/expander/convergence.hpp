#pragma once

#include "../../../type/ntype.hpp"

namespace nm {
    class convergence: public instance {
        NM(ADT(convergence, instance))

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
        virtual nbool converge() const = 0;
    };
}
