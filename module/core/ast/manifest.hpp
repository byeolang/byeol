#pragma once

#include "core/ast/validable.hpp"

namespace by {

    // TODO: entrypoints[== filePath] can be various. but not language.
    struct _nout entrypoint: public validable {
        entrypoint(const std::string& newLang, const std::vector<std::string>& newPaths);
        virtual ~entrypoint();

    public:
        nbool isValid() const override;

    public:
        std::string lang;
        std::vector<std::string> paths;
    };

    typedef std::vector<entrypoint> entrypoints;

    struct manifest: public validable {
        static constexpr const nchar* DEFAULT_NAME = "{default}";

        manifest();
        manifest(const std::string& newName);
        manifest(const std::string& newName, const std::string& newFilePath, const std::string& newAuthor,
            const std::string& newVer, const entrypoints& newPoints);
        virtual ~manifest();

        nbool isValid() const override;

    public:
        std::string name; // manifest name should not contain '.'(dot) character.
        std::string filePath;
        std::string author;
        std::string version;
        std::string ver;

        entrypoints points;
    };

    typedef std::vector<manifest> manifests;
} // namespace by
