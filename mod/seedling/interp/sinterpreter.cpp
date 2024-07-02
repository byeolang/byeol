#include "seedling.hpp"
#include "../parser/seedlingParser.hpp"
#include <fstream>
#include "flex.hpp"

void yyrestart(FILE*);

namespace nm {

    NAMU_DEF_ME(sinterpreter)

    tstr<sobj> me::interp(const nchar* script) {
        YY_BUFFER_STATE buffer = yy_scan_string(script);
        tstr<sobj> ret = _runParser();
        yy_delete_buffer(buffer);
        return ret;
    }

    tstr<sobj> me::interp(const std::string& script) {
        return interp(script.c_str());
    }

    tstr<sobj> me::interpFile(const nchar* path) {
        yyin = fopen(path, "r");
        yyrestart(yyin);
        if(!yyin)
            return NAMU_E("invalid file path %s.", path), tstr<sobj>();

        std::string fileName = _extractFileName(path);
        NAMU_I("interpreting file '%s'...", fileName.c_str());

        tstr<sobj> ret = _runParser();

        ret->setName(fileName);
        fclose(yyin);
        NAMU_I("%s seedling file interpreted.", fileName.c_str());
        return ret;
    }

    tstr<sobj> me::interpFile(const std::string& path) {
        return interpFile(path.c_str());
    }

    tstr<sobj> me::_runParser() {
        int res = yyparse();
        if(res)
            return NAMU_E("interpretion has been failed. res=%d", res), tstr<sobj>();
        if(!root)
            NAMU_E("nothing interpreted.");

        return tstr<sobj>(root);
    }

    std::string me::_extractFileName(const std::string& path) {
        size_t atSlash = path.find_last_of("\\/");
        if(atSlash < 0) return "";

        return path.substr(atSlash + 1);
    }
}
