/// @file
#pragma once

#include "core/builtin/container/native/tnchain.inl"
#include "core/ast/node.hpp"

namespace by {

    /// @ingroup core
    /// @brief Source code file information
    /// @details Literally contains information about the source code file that is the origin of the AST. Used as a
    /// core feature in the byeol language's Context-based REPL.
    class _nout srcFile: public node {
        BY(CLASS(srcFile, node), VISIT())

    public:
        srcFile(const std::string& fileName, const std::string& contents);
        srcFile();

    public:
        using super::prioritize;
        priorType prioritize(const args& a) const override;

        using super::eval;
        str eval(const args& a) override;

        scope& subs() override;
        const std::string& getFileName() const;
        const std::string& getContents() const;

    private:
        std::string _fileName;
        std::string _contents;
    };
}
