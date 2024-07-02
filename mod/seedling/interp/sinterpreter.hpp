#pragma once

#include "../ast.hpp"

namespace nm {

    class _nout sinterpreter {
        NAMU_ME(sinterpreter)

    public:
        static tstr<sobj> interp(const nchar* script);
        static tstr<sobj> interp(const std::string& script);
        static tstr<sobj> interpFile(const nchar* path);
        static tstr<sobj> interpFile(const std::string& path);
        static const std::string& getFileName();

    private:
        static std::string _extractFileName(const std::string& path);
        static tstr<sobj> _runParser();
    };
}
