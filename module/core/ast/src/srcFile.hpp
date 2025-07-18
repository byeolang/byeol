#pragma once

#include "core/builtin/container/native/tnchain.inl"
#include "core/ast/node.hpp"

namespace by {

    /// srcFile represents src file of byeolang source codes.
    class _nout srcFile: public node {
        BY(CLASS(srcFile, node), VISIT())

    public:
        srcFile(const std::string& fileName, const std::string& contents);
        srcFile();

    public:
        using super::prioritize;
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
