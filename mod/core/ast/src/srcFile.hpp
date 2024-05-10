#pragma once

#include "../node.hpp"
#include "../../builtin/container/native/tnchain.inl"

namespace namu {

    /// srcFile represents src file of namulang source codes.
    class _nout srcFile : public node {
        NAMU(CLASS(srcFile, node), VISIT())

    public:
        srcFile(const std::string& fileName, const std::string& contents);

    public:
        priority prioritize(const args& a) const override;

        using super::run;
        str run(const args& a) override;
        nbicontainer& subs() override;
        const std::string& getFileName() const;
        const std::string& getContents() const;

    private:
        std::string _fileName;
        std::string _contents;
    };
}
