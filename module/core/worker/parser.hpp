/// @file
#pragma once

#include "core/ast/exprs/FBOExpr.hpp"
#include "core/ast/exprs/exprMaker.hpp"
#include "core/ast/node.hpp"
#include "core/ast/origin.hpp"
#include "core/ast/params.hpp"
#include "core/ast/pack.hpp"
#include "core/ast/func.hpp"
#include "core/builtin/err/errReport.hpp"
#include "core/worker/bison/tokenScan.hpp"
#include "core/worker/defBlock.hpp"
#include "core/worker/smartDedent.hpp"
#include "core/worker/supply/srcSupply.hpp"
#include "core/worker/tokenDispatcher.hpp"
#include "core/worker/tworker.hpp"

namespace by {

    class expr;
    class ctor;
    class blockExpr;
    class evalExpr;
    class asExpr;
    class isExpr;
    class addExpr;
    class getExpr;
    class retExpr;
    class breakExpr;
    class nextExpr;
    class ifExpr;
    class FUOExpr;
    class defPropExpr;
    class genericOrigin;
    class defNestedFuncExpr;
    class endExpr;

    /** @ingroup core
     *  @brief Parser for byeol language source code
     *  @details Entry point for the byeol parsing component, inheriting from @ref worker. `work()` returns the parsed
     *  result as @ref pack.
     *
     *  @section similar_structure_to_stela_parser Similar Structure to Stela Parser
     *  Since the stela language itself is part of the byeol language, its parser is also based on the byeol language
     *  parser. The stela parser is less complex compared to the byeol parser, so it's recommended to examine the stela
     *  parser code first before looking at the core module.
     *
     *  @section scanner_bison_parser_structure Scanner - Bison - Parser Structure
     *  Uses flex and bison, naming flex as lowscanner and bison as lowparser. These low-level scanner and parser only
     *  exist inside the parser component and are not exposed externally. When parser::work() executes, it runs
     *  lowscanner, which tokenizes and passes tokens to lowparser. Lowparser matches rules and passes those events back
     *  to parser. Therefore, parser's functions starting with `on` are event handling functions that define how to
     *  create nodes and construct the AST.
     *
     *  @section lowparser_lifecycle_management lowparser.y Lifecycle Management
     *  In each rule, calling parser::onXXXX() functions often creates new objects on the heap using the `new` keyword
     *  (not all functions do this). If these arguments are placed directly into @ref tbicontainer, that's fine, but
     *  otherwise they must be bound with @ref tstr beforehand to avoid memory leaks.
     *
     *  @code
     *      // - lowparser.y
     *      pack: PACK name-access NEWLINE {
     *          $$ = PS.onPack(*$2); // onPack() returns new pack()
     *      }
     *
     *      compilation-unit: pack defblock {
     *          tstr<obj> pak($1); // Without binding with tstr like this,
     *
     *          PS.onCompilationUnit(pak.get()); // if the pak value has issues inside onCompilationUnit()
     *                                           // and the operation is cancelled, the pack object
     *                                           // created on the heap becomes a memory leak.
     *      }
     *  @endcode
     *
     *  This applies not only to onXXXX() functions but also to string tokens. See the following code:
     *
     *  @code
     *      // -- lowscanner.l
     *      <stateString>\"       { // When string scan ends
     *          if(!yylval->asStr) yylval->asStr = new std::string(); // Creates string object with new
     *          ....
     *          return STRVAL;
     *      }
     *
     *      // -- lowparser.y
     *          ...
     *         | STRVAL { // When STRVAL comes from scanner
     *          $$ = PS.onPrimitive<nStr>(*$1); // Pass it first. String is always immutable in this project
     *                                          // so the object is always copied.
     *          delete $1; // Since the object is copied, delete the original object.
     *  @endcode
     *
     *  @section indentation_rule Indentation Rule
     *  The byeol language applies the offside rule, making it very sensitive to indentation. Unlike typical languages,
     *  it must be able to count how many spaces exist immediately after a newline. Once indentation is determined and
     *  the code line's scope is confirmed, subsequent spaces should be ignored. Consider the following byeol language
     *  example:
     *
     *  @code
     *  def A
     *      foo(val int) void
     *          if val > 0
     *            if val < 5
     *                    print("0 < val < 5")
     *          print("end of func") # 1)
     *  @endcode
     *
     *  After parsing `print("0 < val < 5")`, when parsing the next line `print("end of func")`, the parser must count
     *  the leading spaces and compare with the space counts for each scope to determine which scope this code line
     *  belongs to. Looking at the example, we can intuitively see that `print("end of func")` has the same spacing as
     *  `if val > 0`, meaning it belongs directly inside the foo() body. This means at this point, both the scope inside
     *  `if val > 0` and the scope inside `if val < 5` are closed. So lowscanner must add 2 `DEDENT` tokens (signifying
     *  scope termination) before lowparser recognizes `print("end of func")`.
     *
     *  @section token_dispatcher tokenDispatcher
     *  Flex uses a separately designated stream called yyin to get characters and define them as tokens. As shown in
     *  the example above, adding tokens during parsing means adding specific characters to this stream. Typically unput
     *  is used for this, but to flexibly handle multiple unputs or pushing to the front, lowscanner internally uses
     *  @ref tokenDispatcher.
     *
     *  @section token_scan tokenScan
     *  As explained earlier, detecting indentation is crucial. Accurate space counting is required, so a strategy
     *  pattern is applied, switching between @ref normalScan and @ref indentScan. When a newline is detected, it
     *  switches to indentScan to count spaces accurately and determine scope.
     */
    class _nout parser: public tworker<str, pack>, public tokenScanable {
        typedef tworker<str, pack> __super5;
        BY(CLASS(parser, __super5))
        friend class coreInternal;

    public:
        parser();

    public:
        /**
         * @brief Retrieves the root node of the parsed sub-pack.
         * @details This method provides access to the top-level AST node representing the entire parsed sub-pack.
         * @return A pointer to the root node of the sub-pack.
         */
        node* getSubPack();
        const node* getSubPack() const BY_CONST_FUNC(getSubPack())

        srcSupplies& getSrcSupplies();
        const srcSupplies& getSrcSupplies() const BY_CONST_FUNC(getSrcSupplies())
        me& addSupply(const srcSupply& new1);
        me& addSupply(const tucontainable<srcSupply>& new1);
        me& relSupplies();

        tokenDispatcher& getDispatcher();
        std::vector<ncnt>& getIndents();

        nbool isInit() const;

        /**
         * @brief Sets the scanning mode for the lexer.
         * @tparam T The type of the scanner strategy (e.g., `tokenScan`, `indentScan`).
         * @note Logs the change in scan mode for debugging purposes.
         */
        template <typename T> void setScan() {
            BY_DI("change scanmode(%s -> %s)", _mode ? _mode->getType().getName() : "null", T::instance);
            _mode = &T::instance;
        }

        void rel() override;

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
        using tokenScanable::onScan;
        /**
         * @brief Handles scanning for the lexer, overriding the base `tokenScanable` behavior.
         * @param ps The `parser` instance.
         * @param val The YYSTYPE value pointer for the token.
         * @param loc The YYLTYPE location pointer for the token.
         * @param scanner The Flex scanner instance.
         * @param isBypass Flag indicating if indentation bypass is active.
         * @return The token ID.
         */
        nint onScan(parser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner, nbool& isBypass) override;
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
         * @brief Handles the left parenthesis token.
         * @param tok The token ID of the left parenthesis.
         * @return The token ID.
         */
        nint onTokenLParan(nint tok);
        /**
         * @brief Handles the right parenthesis token.
         * @param tok The token ID of the right parenthesis.
         * @return The token ID.
         */
        nint onTokenRParan(nint tok);
        /**
         * @brief Handles the comma token.
         * @param tok The token ID of the comma.
         * @return The token ID.
         */
        nint onTokenComma(nint tok);
        /**
         * @brief Handles the end of a braces block (e.g., `}`).
         * @return true if the end of braces was handled, false otherwise.
         */
        nbool onTokenEndOfBraces();
        /**
         * @brief Handles the start of a string template brace (e.g., `${`).
         * @note This method is likely related to parsing interpolated strings.
         */
        void onTokenStartOfStrTemplateBrace();
        /**
         * @brief Handles the start of a string literal.
         * @details Stores the opening quote character ('"' or '\'') to distinguish
         *          between double-quoted and single-quoted strings during parsing.
         *          This enables support for both quote types using the same scanner state.
         * @param token The opening quote character ('"' or '\'')
         */
        void onTokenBeginStr(nchar token);
        /**
         * @brief returns quote character which used to start current string sequence.
         * @return quote or double quote to be used to open string literal.
         */
        nchar getOpenStrToken();
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
        /**
         * @brief Callback for processing source area information from the lexer.
         * @param area The source area (e.g., line, column) information.
         */
        void onSrcArea(const area& area);

        //  err:
        //  operator:
        //      dot:
        node* onGet(const std::string& name);
        node* onGet(const std::string& name, const narr& args);
        node* onGet(node& from, const std::string& name);
        node* onGet(node& from, node& it);
        node* onGet(const std::string& name, const std::string& name2);
        node* onCallAccess(node& it, const narr& args);

        //      unary operation:
        FBOExpr* onUnaryMinus(const node& it);
        FBOExpr* onUnaryNot(const node& it);
        FUOExpr* onUnaryBitwiseNot(const node& it);
        node* onUnaryDoublePlus(node& it);
        node* onUnaryDoubleMinus(node& it);
        FUOExpr* onUnaryPostfixDoublePlus(const node& it);
        FUOExpr* onUnaryPostfixDoubleMinus(const node& it);
        //      binary operation:
        //          arithmetic:
        FBOExpr* onAdd(const node& lhs, const node& rhs);
        FBOExpr* onSub(const node& lhs, const node& rhs);
        FBOExpr* onMul(const node& lhs, const node& rhs);
        FBOExpr* onDiv(const node& lhs, const node& rhs);
        FBOExpr* onMod(const node& lhs, const node& rhs);
        //          bitwise:
        FBOExpr* onBitwiseAnd(const node& lhs, const node& rhs);
        FBOExpr* onBitwiseXor(const node& lhs, const node& rhs);
        FBOExpr* onBitwiseOr(const node& lhs, const node& rhs);
        FBOExpr* onLShift(const node& lhs, const node& rhs);
        FBOExpr* onRShift(const node& lhs, const node& rhs);
        //          assign:
        node* onAssign(node& lhs, node& rhs);
        node* onAddAssign(node& lhs, node& rhs);
        node* onSubAssign(node& lhs, node& rhs);
        node* onMulAssign(node& lhs, node& rhs);
        node* onDivAssign(node& lhs, node& rhs);
        node* onModAssign(node& lhs, node& rhs);
        //          logical:
        FBOExpr* onGt(const node& lhs, const node& rhs);
        FBOExpr* onGe(const node& lhs, const node& rhs);
        FBOExpr* onLt(const node& lhs, const node& rhs);
        FBOExpr* onLe(const node& lhs, const node& rhs);
        FBOExpr* onEq(const node& lhs, const node& rhs);
        FBOExpr* onNe(const node& lhs, const node& rhs);
        FBOExpr* onAnd(const node& lhs, const node& rhs);
        FBOExpr* onOr(const node& lhs, const node& rhs);

        //  generic:
        node* onGetGeneric(const std::string& orgName, const args& typeParams);
        node* onGetElem(const node& arr, const node& idx);
        //  container:
        node* onGetArray(node& elemType);

        //  keyword:
        obj* onPack(const node& path);
        obj* onPack(const std::string& path);
        obj* onPack();
        obj* onSubPack(obj& subpack);
        endExpr* onEnd(const blockExpr& blk);
        blockExpr* onBlock(const node* stmt);
        blockExpr* onBlock(blockExpr* blk, const node* stmt);
        blockExpr* onBlock();
        defBlock* onDefBlock(node* stmt);
        defBlock* onDefBlock(defBlock* blk, node* stmt);
        defBlock* onDefBlock();
        node* onFor(const std::string& iterName, const node& expr, const blockExpr& blk);
        node* onWhile(const node& condition, const blockExpr& blk);

        //  expr:
        //      def:
        //          list:
        node* onParanthesisAsTuple(narr& tuple);
        args* onTuple();
        args* onTuple(const node& elem);
        args* onTuple(args& as, const node& elem);
        args* onFuncCallTuple();
        args* onFuncCallTuple(const node& elem);
        args* onFuncCallTuple(args& as, const node& elem);
        //          typenames:
        args* onTypeNames(const node& param);
        args* onTypeNames(args& params, const node& param);

        //          var:
        template <typename T, typename Arg> T* onPrimitive(const Arg& arg) {
            BY_DI("on%s(...)", ttype<T>::get());
            return new T(arg);
        }

        template <typename T> T* onPrimitive() {
            BY_DI("on%s()", ttype<T>::get());
            return new T();
        }

        node* onDefProp(const modifier& mod, const std::string& name, const node& rhs);
        node* onDefProp(const std::string& name, const node& rhs);
        node* onDefProp(const node& rhs);
        node* onDefProp(const modifier& mod, const node& rhs);
        node* onDefAssign(const modifier& mod, const std::string& name, const node* rhs);
        node* onDefAssign(const defPropExpr* prop, const node* rhs);
        node* onDefAssign(const std::string& name, const node* rhs);
        //          obj:
        obj* onDefOrigin(const std::string& name, defBlock& blk);
        obj* onDefOrigin(const std::string& name, const narr& args, defBlock& blk);
        genericOrigin* onDefObjGeneric(const std::string& name, const args& typeParams, defBlock& blk);
        genericOrigin* onDefObjGeneric(const std::string& name, const args& typeParams, const narr& args,
            defBlock& blk);
        //          container:
        node* onDefArray(const narr& items);
        node* onDefSeq(const node& start, const node& end);
        //          file:
        void onCompilationUnit(obj* subpack);
        void onCompilationUnit(obj* subpack, defBlock* blk);
        //          func:
        func* onFuncSignature(const modifier& mod, const getExpr& access, const node* retType);
        func* onFuncSignature(const getExpr& access, const node* retType);
        func* onFuncSignature(const modifier& mod, node& it, const node* retType);
        func* onFuncSignature(node& it, const node* retType);
        func* onAbstractFunc(func& f);
        node* onFunc(func& func, const blockExpr& blk);
        defNestedFuncExpr* onLambda(const narr& params, const node& retType, const blockExpr& blk);
        ctor* onCtor(const modifier& mod, const narr& args, const blockExpr& blk);
        ctor* onCtor(const narr& args, const blockExpr& blk);
        ctor* onCtor(const modifier& mod, const blockExpr& blk);
        ctor* onCtor(const blockExpr& blk);
        //          params:
        narr* onParams();
        narr* onParams(const defPropExpr* elem);
        narr* onParams(narr& it, const defPropExpr* elem);
        //          modifier:
        modifier* onModifier(nbool isPublic, nbool isOverriden);

        // stmt:
        node* onDeclStmt(const narr& dotnames) {
            // TODO:
            return nullptr;
        }

        //      return:
        retExpr* onRet();
        retExpr* onRet(node& exp);
        breakExpr* onBreak();
        nextExpr* onNext();
        //      eval:
        evalExpr* onFillFromOfFuncCall(const node& me, evalExpr& to);
        evalExpr* onEvalExpr(node& type, const narr& a);
        evalExpr* onEvalExpr(node& type, const args& a);
        //      cast:
        isExpr* onIs(const node& me, const node& to);
        asExpr* onAs(const node& me, const node& as);
        //      if:
        ifExpr* onIf(const node& condition, const blockExpr& then);
        ifExpr* onIf(const node& condition, const blockExpr& then, const blockExpr& elseBlk);
        ifExpr* onIf(const node& condition, const blockExpr& then, const ifExpr& elseIf);
        //      in:
        evalExpr* onIn(const node& it, const node& container);

        /**
         * @brief Callback invoked at the end of parsing a function definition.
         * @details This method handles any cleanup or finalization required after a function's
         *          body has been parsed.
         */
        void onEndFunc();
        /**
         * @brief Reports a parsing error.
         * @param msg The error message.
         * @param symbolName The name of the symbol related to the error.
         */
        void onParseErr(const std::string& msg, const nchar* symbolName);

    protected:
        void* _scanString(const std::string& src, void* scanner);
        exprMaker& _getMaker();
        void _prepare() override;
        void _onEndWork() override;

        /**
         *  parse with given srcSupply instances.
         *  @param script is null terminated cstring.
         *  @return last parsed sub pack.
         */
        str _onWork() override;

    private:
        node* _onDefAssign(const modifier& mod, const node* type, const std::string& name, const node* rhs);
        nint _onTokenEndOfInlineBlock(nint tok);
        node* _onSetElem(evalExpr& lhs, const node& rhs);
        node* _onAssignElem(FBOExpr::symbol type, node& lhs, node& rhs);
        node* _onConvertAssignElem(evalExpr& lhs, node& rhs);
        evalExpr* _onEvalExpr(node* me, node& type, const args& a);

        nint _onScan(YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner);
        void _report(baseErr* new1);
        params _asParams(const args& exprs);

        nbool _onInjectObjSubs(obj& it, defBlock& blk);
        nbool _onInjectCtor(obj& it, defBlock& blk);
        std::vector<std::string> _extractParamTypeNames(const args& types);
        std::string _joinVectorString(const std::vector<std::string>& container) const;
        const node& _onDefArrayType(const narr& items);
        std::vector<std::string> _toDotnames(const node& path);
        void _onCompilationUnit(obj* subpack, defBlock* blk);
        tstr<modifier> _makeDefaultModifier();

    private:
        tokenScan* _mode;
        nbool _isIgnoreWhitespace;
        tokenDispatcher _dispatcher;
        std::vector<ncnt> _indents;
        str _subpack;
        tstr<scope> _filescope;
        std::vector<nint> _states;
        exprMaker _maker;
        smartDedent _dedent;
        srcSupplies _supplies;
        std::vector<func*> _funcs;
        ncnt _strTemplateCnt;
        nchar _strOpenToken;
    };
} // namespace by
