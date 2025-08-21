/// @file
#pragma once

#include "leaf/ast.hpp"
#include "leaf/parser/leafTokenScanable.hpp"
#include "leaf/parser/leafTokenDispatcher.hpp"
#include "leaf/parser/bison/leafTokenScan.hpp"
#include "leaf/parser/leafSmartDedent.hpp"

namespace by {
    class leafTokenScan;

    /// @ingroup leaf
    /// @brief Parser for leaf configuration language
    /// @details Main parser class that converts leaf language source code into AST.
    /// Handles tokenization, parsing, and error reporting for the leaf configuration format.
    class _nout leafParser: public leafTokenScanable {
        BY(ME(leafParser))

    public:
        leafParser();

    public:
        tstr<leaf> parse(const std::string& codes);
        tstr<leaf> parse(const nchar* codes);
        tstr<leaf> parseFromFile(const std::string& path);
        tstr<leaf> parseFromFile(const nchar* path);

        leafTokenDispatcher& getDispatcher();
        std::vector<ncnt>& getIndents();

        nbool isInit() const;

        template <typename T> void setScan() { _mode = T::_instance; }

        void rel();

        int pushState(int newState);
        int popState();

        // events:
        //  scan:
        using leafTokenScanable::onScan;
        nint onScan(leafParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner, nbool& isBypass) override;
        nint onTokenEndOfFile();
        nint onTokenColon(nint tok);
        nint onTokenNewLine(nint tok);
        nint onTokenComma(nint tok);
        nint onIndent(ncnt col, nint tok);
        nint onDedent(ncnt col, nint tok);
        nint onIgnoreIndent(nint tok);
        nchar onScanUnexpected(const nchar* token);

        //  keyword:
        leaf* onDefBlock(leaf* stmt);
        leaf* onDefBlock(leaf* blk, leaf* stmt);
        leaf* onDefBlock();

        //  expr:
        //      def:
        //          var:
        template <typename T> leaf* onPrimitive(const T& arg) { return new valLeaf(arg); }

        verLeaf* onVer(const std::string& version);

        leaf* onDefProp(const std::string& name, leaf& rhs);
        leaf* onDefAssign(const std::string& name, leaf* rhs);
        //          obj:
        leaf* onDefOrigin(const std::string& name, leaf& blk);
        //          container:
        leaf* onDefArray(const leaf& elem);
        leaf* onDefArray(leaf& as, const leaf& elem);
        //          file:
        leaf* onCompilationUnit(leaf* blk);

        void onParseErr(const std::string& msg, const nchar* symbolName);
        void report(const std::string& msg);

    private:
        void _prepare();
        void* _scanString(const nchar* src, void* scanner);
        nint _onTokenEndOfInlineBlock(nint tok);
        nint _onScan(ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner);
        tstr<leaf> _finalize();

    private:
        leafTokenScan* _mode;
        nbool _isIgnoreWhitespace;
        leafTokenDispatcher _dispatcher;
        std::vector<ncnt> _indents;
        tstr<leaf> _root;
        std::vector<nint> _states;
        leafSmartDedent _dedent;
        std::vector<std::string> _errs;
    };
} // namespace by
