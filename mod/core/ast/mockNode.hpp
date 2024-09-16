#pragma once

#include "node.hpp"

namespace nm {

    class _nout mockNode: public node {
        NM(ME(mockNode, node), VISIT())
        friend class exprMaker;

    public:
        mockNode();
        mockNode(const node& org);

    public:
        const ntype& getType() const override;

        using super::subs;
        scope& subs() override;

        priorType prioritize(const args& a) const override;

        str run(const args& a) override;

        clonable* clone() const override;

        nbool isComplete() const override;

        const src& getSrc() const override;

        void* cast(const type& to) override;

        clonable* cloneDeep() const override;

        void onCloneDeep(const clonable& from) override;

    protected:
        void _setSrc(const src& s) override;

    private:
        str _org;
    };
}
