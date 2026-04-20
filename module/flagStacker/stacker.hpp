#pragma once

#include "flagStacker/flag/flagable.hpp"
#include "flagStacker/flag/flag.hpp"

namespace by {
    class _nout stacker: public flagable {
    public:
        ~stacker();

    public:
        using flagable::take;
        res take(flagArgs& a);

        nbool canTake(const std::string& pattern) const override;

        const flags& getFlags() const;

    protected:
        virtual void _initFlags(flags& tray) const = 0;

    private:
        res take(flagArgs& a, stacker& s) override;

    private:
        mutable flags _flags;
    };
}
