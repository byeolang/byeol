#pragma once

#include "../../builtin/container/native/tnchain.inl"
#include "../node.hpp"

namespace nm {

    /// srcFile represents src file of namulang source codes.
    class _nout srcFile: public node {
        NM(CLASS(srcFile, node), VISIT())

    public:
        srcFile(const std::string& fileName, const std::string& contents);

    public:
        priorType prioritize(const args& a) const override;

        using super::run;
        str run(const args& a) override;
        scope& subs() override;
        const std::string& getFileName() const;
        const std::string& getContents() const;

    private:
        std::string _fileName;
        std::string _contents;
    };
}
