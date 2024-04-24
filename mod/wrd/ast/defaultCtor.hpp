#pragma once

#include "ctor.hpp"

namespace wrd {

    class _wout defaultCtor : public ctor {
        WRD(CLASS(defaultCtor, ctor))

    public:
        defaultCtor(const node& org): super(org) {}

    public:
        using super::run;
        str run(const ucontainable& args) override {
            if(args.len() >= 1) return str();

            return str((node*) getOrigin().clone());
        }
    };
}
