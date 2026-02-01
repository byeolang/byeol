/// @file
#pragma once

#include "stela/ast.hpp"
#include "stela/parser/stelaTokenScanable.hpp"
#include "stela/parser/stelaTokenDispatcher.hpp"
#include "stela/parser/bison/stelaTokenScan.hpp"
#include "stela/parser/stelaSmartDedent.hpp"

namespace by {
    class stelaTokenScan;

    /** @ingroup stela
     *  @brief Parser for stela configuration language
     *  @details Entry point for the @ref stela parsing component. Specify scripts through
     *  `parse()` or `parseFromFile()`, and the parsed result is returned in @ref stela
     *  structure.
     *
     *  @section similar_structure Similar structure to byeol parser
     *  Since the stela language itself is a specialized language of byeol, the parser is
     *  also based on the byeol language parser. Since it's a less complex language than
     *  byeol, it's recommended to examine this parser code before looking at the core module.
     *
     *  @section scanner - bison - stelaParser structure
     *  Uses flex and bison, with flex named lowscanner and bison named lowparser. These
     *  low-level scanner and parser exist only within the parser component and are never
     *  exposed externally. When stelaParser::parse() executes, it runs lowscanner.
     *  lowscanner tokenizes and passes tokens to lowparser. When lowparser matches rules
     *  on received tokens, it passes events back to stelaParser. Therefore, stelaParser's
     *  functions starting with `on` are event handling functions that define how to create
     *  nodes and build the AST.
     *
     *  @section indentation_rule Indentation rule
     *  Like the byeol language, stela applies the offside rule, making it very sensitive to
     *  indentation. Unlike typical languages, it must count whitespace after newlines.
     *  Once indentation is confirmed and determines which scope the code line belongs to,
     *  whitespace should be ignored afterward.
     */
    class _nout stelaParser: public stelaTokenScanable {
        BY(ME(stelaParser))

    public:
        stelaParser();

    public:
        /**
         * @brief Parses the given string of stela code.
         * @param codes The stela code as a string.
         * @return A tstr to the parsed stela AST root object, or null on error.
         */
        tstr<stela> parse(const std::string& codes);
        /**
         * @brief Parses the given C-style string of stela code.
         * @param codes The stela code as a C-style string.
         * @return A tstr to the parsed stela AST root object, or null on error.
         */
        tstr<stela> parse(const nchar* codes);
        /**
         * @brief Parses stela code from the specified file path.
         * @param path The path to the file containing stela code.
         * @return A tstr to the parsed stela AST root object, or null on error.
         */
        tstr<stela> parseFromFile(const std::string& path);
        /**
         * @brief Parses stela code from the specified C-style file path.
         * @param path The path to the file containing stela code.
         * @return A tstr to the parsed stela AST root object, or null on error.
         */
        tstr<stela> parseFromFile(const nchar* path);

        stelaTokenDispatcher& getDispatcher();
        std::vector<ncnt>& getIndents();

        nbool isInit() const;

        /**
         * @brief Sets the scanning mode for the lexer.
         * @tparam T The type of the scanner strategy (e.g., normalScan, indentScan).
         */
        template <typename T> void setScan() { _mode = &T::instance; }

        void rel();

        /**
         * @brief Pushes a new Flex scanner state onto the state stack.
         * @param newState The new state to push.
         * @return The new state.
         */
        int pushState(int newState);
        /**
         * @brief Pops a Flex scanner state from the state stack.
         * @return The previous state.
         */
        int popState();

        // events:
        //  scan:
        using stelaTokenScanable::onScan;
        /**
         * @brief Handles scanning for the lexer, overriding the base stelaTokenScanable behavior.
         * @param ps The stelaParser instance.
         * @param val The YYSTYPE value pointer for the token.
         * @param loc The YYLTYPE location pointer for the token.
         * @param scanner The Flex scanner instance.
         * @param isBypass Flag indicating if indentation bypass is active.
         * @return The token ID.
         */
        nint onScan(stelaParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner, nbool& isBypass) override;
        /**
         * @brief Handles the end-of-file token.
         * @return The token ID for end-of-file.
         */
        nint onTokenEndOfFile();
        /**
         * @brief Handles the colon token.
         * @param tok The token ID of the colon.
         * @return The token ID.
         */
        nint onTokenColon(nint tok);
        /**
         * @brief Handles the newline token.
         * @param tok The token ID of the newline.
         * @return The token ID.
         */
        nint onTokenNewLine(nint tok);
        /**
         * @brief Handles the comma token.
         * @param tok The token ID of the comma.
         * @return The token ID.
         */
        nint onTokenComma(nint tok);
        /**
         * @brief Handles an increase in indentation.
         * @param col The column count of the new indentation level.
         * @param tok The token ID that triggered the indentation.
         * @return The token ID for INDENT.
         */
        nint onIndent(ncnt col, nint tok);
        /**
         * @brief Handles a decrease in indentation.
         * @param col The column count of the new indentation level.
         * @param tok The token ID that triggered the dedentation.
         * @return The token ID for DEDENT.
         */
        nint onDedent(ncnt col, nint tok);
        /**
         * @brief Handles ignoring indentation (e.g., for inline blocks).
         * @param tok The token ID.
         * @return The token ID.
         */
        nint onIgnoreIndent(nint tok);
        /**
         * @brief Handles an unexpected token encountered during scanning.
         * @param token The unexpected token string.
         * @return The token ID for an error.
         */
        nchar onScanUnexpected(const nchar* token);

        //  keyword:
        stela* onDefBlock(stela* stmt);
        stela* onDefBlock(stela* blk, stela* stmt);
        stela* onDefBlock();

        //  expr:
        //      def:
        //          var:
        /**
         * @brief Creates a @ref valStela object from a primitive value.
         * @tparam T The primitive type of the argument (e.g., int, bool, string).
         * @param arg The primitive value.
         * @return A pointer to a newly created @ref valStela instance.
         */
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

        /**
         * @brief Handles a parse error, reporting the message and the symbol where the error occurred.
         * @param msg The error message.
         * @param symbolName The name of the symbol causing the error.
         */
        void onParseErr(const std::string& msg, const nchar* symbolName);
        /**
         * @brief Reports a message, typically an error or warning, encountered during parsing.
         * @param msg The message to report.
         */
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
