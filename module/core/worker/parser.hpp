/// @file
#pragma once

#include "core/ast/exprs/FBOExpr.hpp"
#include "core/ast/exprs/exprMaker.hpp"
#include "core/ast/node.hpp"
#include "core/ast/origin.hpp"
#include "core/ast/params.hpp"
#include "core/ast/slot.hpp"
#include "core/ast/func.hpp"
#include "core/builtin/err/errReport.hpp"
#include "core/worker/bison/tokenScan.hpp"
#include "core/worker/defBlock.hpp"
#include "core/worker/smartDedent.hpp"
#include "core/worker/supply/srcSupply.hpp"
#include "core/worker/tokenDispatcher.hpp"
#include "core/worker/worker.hpp"

namespace by {

    class expr;
    class ctor;
    class blockExpr;
    class runExpr;
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

    /// @ingroup core
    /// @brief Parser for byeol language source code
    /// @details Main parser class that converts source code into abstract syntax trees.
    /// Handles tokenization, syntax analysis, and AST construction for the byeol language.
    class _nout parser: public worker<str, slot>, public tokenScanable {
        typedef worker<str, slot> __super5;
        BY(CLASS(parser, __super5))
        friend class srcSupply;

    public:
        parser();

    public:
        node* getSubPack();
        const node* getSubPack() const BY_CONST_FUNC(getSubPack())

        srcSupplies& getSrcSupplies();
        const srcSupplies& getSrcSupplies() const BY_CONST_FUNC(getSrcSupplies())
        me& addSupply(const srcSupply& new1);
        me& relSupplies();

        tokenDispatcher& getDispatcher();
        std::vector<ncnt>& getIndents();

        nbool isInit() const;

        template <typename T> void setScan() {
            BY_DI("change scanmode(%s -> %s)", _mode ? _mode->getType().getName() : "null", *T::_instance);
            _mode = T::_instance;
        }

        void rel() override;

        int pushState(int newState);
        int popState();

        // events:
        //  scan:
        using tokenScanable::onScan;
        nint onScan(parser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner, nbool& isBypass) override;
        nint onTokenEndOfFile();
        nint onTokenColon(nint tok);
        nint onTokenNewLine(nint tok);
        nint onTokenLParan(nint tok);
        nint onTokenRParan(nint tok);
        nint onTokenComma(nint tok);
        nbool onTokenEndOfBraces();
        void onTokenStartOfStrTemplateBrace();
        nint onIndent(ncnt col, nint tok);
        nint onDedent(ncnt col, nint tok);
        nint onIgnoreIndent(nint tok);
        nchar onScanUnexpected(const nchar* token);
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
        //      run:
        runExpr* onFillFromOfFuncCall(const node& me, runExpr& to);
        runExpr* onRunExpr(node& type, const narr& a);
        runExpr* onRunExpr(node& type, const args& a);
        //      cast:
        isExpr* onIs(const node& me, const node& to);
        asExpr* onAs(const node& me, const node& as);
        //      if:
        ifExpr* onIf(const node& condition, const blockExpr& then);
        ifExpr* onIf(const node& condition, const blockExpr& then, const blockExpr& elseBlk);
        ifExpr* onIf(const node& condition, const blockExpr& then, const ifExpr& elseIf);
        //      in:
        runExpr* onIn(const node& it, const node& container);

        void onEndFunc();
        void onParseErr(const std::string& msg, const nchar* symbolName);

    protected:
        void* _scanString(const std::string& src, void* scanner);
        exprMaker& _getMaker();
        void _prepare() override;
        void _onEndWork() override;

        /// parse with given srcSupply instances.
        /// @param script is null terminated cstring.
        /// @return last parsed sub pack.
        str _onWork() override;

    private:
        node* _onDefAssign(const modifier& mod, const node* type, const std::string& name, const node* rhs);
        nint _onTokenEndOfInlineBlock(nint tok);
        node* _onSetElem(runExpr& lhs, const node& rhs);
        node* _onAssignElem(FBOExpr::symbol type, node& lhs, node& rhs);
        node* _onConvertAssignElem(runExpr& lhs, node& rhs);
        runExpr* _onRunExpr(node* me, node& type, const args& a);

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
    };
} // namespace by
