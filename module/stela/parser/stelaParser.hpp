/// @file
#pragma once

#include "stela/ast.hpp"
#include "stela/parser/stelaTokenScanable.hpp"
#include "stela/parser/stelaTokenDispatcher.hpp"
#include "stela/parser/bison/stelaTokenScan.hpp"
#include "stela/parser/stelaSmartDedent.hpp"

namespace by {
    class stelaTokenScan;

    /// @ingroup stela
    /// @brief Parser for stela configuration language
    /// @details Main parser class that converts stela language source code into AST.
    /// Handles tokenization, parsing, and error reporting for the stela configuration format.
    class _nout stelaParser: public stelaTokenScanable {
        BY(ME(stelaParser))

    public:
        stelaParser();

    public:
        tstr<stela> parse(const std::string& codes);
        tstr<stela> parse(const nchar* codes);
        tstr<stela> parseFromFile(const std::string& path);
        tstr<stela> parseFromFile(const nchar* path);

        stelaTokenDispatcher& getDispatcher();
        std::vector<ncnt>& getIndents();

        nbool isInit() const;

        template <typename T> void setScan() { _mode = T::_instance; }

        void rel();

        int pushState(int newState);
        int popState();

        // events:
        //  scan:
        using stelaTokenScanable::onScan;
        nint onScan(stelaParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner, nbool& isBypass) override;
        nint onTokenEndOfFile();
        nint onTokenColon(nint tok);
        nint onTokenNewLine(nint tok);
        nint onTokenComma(nint tok);
        nint onIndent(ncnt col, nint tok);
        nint onDedent(ncnt col, nint tok);
        nint onIgnoreIndent(nint tok);
        nchar onScanUnexpected(const nchar* token);

        //  keyword:
        stela* onDefBlock(stela* stmt);
        stela* onDefBlock(stela* blk, stela* stmt);
        stela* onDefBlock();

        //  expr:
        //      def:
        //          var:
        template <typename T> stela* onPrimitive(const T& arg) { return new valStela(arg); }

        verStela* onVer(const std::string& version);

        stela* onDefProp(const std::string& name, stela& rhs);
        stela* onDefAssign(const std::string& name, stela* rhs);
        //          obj:
        stela* onDefOrigin(const std::string& name, stela& blk);
        //          container:
        stela* onDefArray(const stela& elem);
        stela* onDefArray(stela& as, const stela& elem);
        //          file:
        stela* onCompilationUnit(stela* blk);

        void onParseErr(const std::string& msg, const nchar* symbolName);
        void report(const std::string& msg);

    private:
        void _prepare();
        void* _scanString(const nchar* src, void* scanner);
        nint _onTokenEndOfInlineBlock(nint tok);
        nint _onScan(ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner);
        tstr<stela> _finalize();

    private:
        stelaTokenScan* _mode;
        nbool _isIgnoreWhitespace;
        stelaTokenDispatcher _dispatcher;
        std::vector<ncnt> _indents;
        tstr<stela> _root;
        std::vector<nint> _states;
        stelaSmartDedent _dedent;
        std::vector<std::string> _errs;
    };
} // namespace by
