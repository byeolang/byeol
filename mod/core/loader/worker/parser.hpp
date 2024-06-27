#pragma once

#include "../../ast/node.hpp"
#include "tokenDispatcher.hpp"
#include "bison/tokenScan.hpp"
#include "../errReport.hpp"
#include "../../ast/slot.hpp"
#include "../../ast/params.hpp"
#include "defBlock.hpp"
#include "../../ast/exprs/exprMaker.hpp"
#include "../../ast/exprs/FBOExpr.hpp"
#include "smartDedent.hpp"
#include "supply/srcSupply.hpp"
#include "worker.hpp"

namespace namu {

    class expr;
    class func;
    class blockExpr;
    class runExpr;
    class asExpr;
    class addExpr;
    class getExpr;
    class retExpr;
    class breakExpr;
    class nextExpr;
    class ifExpr;
    class FUOExpr;
    class defPropExpr;
    class _nout parser : public worker<tstr<obj>, slot>, public tokenScanable {
        typedef worker<tstr<obj>, slot> __super5;
        NAMU(CLASS(parser, __super5))
        friend class srcSupply;

    public:
        parser();

    public:
        obj& getSubPack();
        const obj& getSubPack() const NAMU_UNCONST_FUNC(getSubPack())

        srcSupplies& getSrcSupplies();
        const srcSupplies& getSrcSupplies() const NAMU_UNCONST_FUNC(getSrcSupplies())
        me& addSupply(const srcSupply& new1);
        me& relSupplies();

        tokenDispatcher& getDispatcher();
        std::vector<ncnt>& getIndents();

        nbool isInit() const;

        template <typename T>
        void setScan() {
            NAMU_DI("change scanmode(%s -> %s)", !_mode ? "<null>" : _mode->getType().getName().c_str(),
                    T::_instance->getType().getName().c_str());
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
        node* onGet(node& from, node& it);
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
        node* onGetGeneric(const std::string& genericObjName, const args& typeParams);
        node* onGetElem(const node& arr, const node& idx);
        //  container:
        node* onGetArray(node& elemType);

        //  keyword:
        obj* onPack(const node& path);
        obj* onPack();
        obj* onSubPack(obj& subpack);
        blockExpr* onBlock(const node& stmt);
        blockExpr* onBlock(blockExpr& blk, const node& stmt);
        blockExpr* onBlock();
        defBlock* onDefBlock(node& stmt);
        defBlock* onDefBlock(defBlock& blk, node& stmt);
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
        template <typename T, typename... Args>
        T* onPrimitive(Args... args) {
            NAMU_DI("on%s(...)", ttype<T>::get().getName().c_str());
            return new T(args...);
        }
        node* onDefProp(const std::string& name, const node& origin);
        //          obj:
        obj* onDefObj(const std::string& name, defBlock& blk);
        node* onDefObjGeneric(const std::string& name, const args& typeParams, defBlock& blk);
        //          container:
        node* onDefArray(const narr& items);
        node* onDefSeq(const node& start, const node& end);
        //          file:
        void onCompilationUnit(obj& subpack);
        void onCompilationUnit(obj& subpack, defBlock& blk);
        //          func:
        func* onAbstractFunc(const getExpr& access, const node& retType);
        func* onAbstractFunc(node& it, const node& retType);
        func* onFunc(func& func, const blockExpr& blk);
        //          params:
        narr* onParams();
        narr* onParams(const defPropExpr& elem);
        narr* onParams(narr& it, const defPropExpr& elem);
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
        asExpr* onAs(const node& me, const node& as);
        //      if:
        ifExpr* onIf(const node& condition, const blockExpr& then);
        ifExpr* onIf(const node& condition, const blockExpr& then, const blockExpr& elseBlk);
        ifExpr* onIf(const node& condition, const blockExpr& then, const ifExpr& elseIf);
        //      in:
        runExpr* onIn(const node& it, const node& container);

        void onParseErr(const std::string& msg, const nchar* symbolName);

        template <typename... Args> void posError(Args... args) { _report(err::newErr(getArea().start, args...)); }
        template <typename... Args> void posWarn(Args... args) { _report(err::newWarn(getArea().start, args...)); }
        template <typename... Args> void posInfo(Args... args) { _report(err::newInfo(getArea().start, args...)); }

    protected:
        void* _scanString(const std::string& src, void* scanner);
        exprMaker& _getMaker();
        void _prepare() override;
        void _onEndWork() override;

        /// parse with given srcSupply instances.
        /// @param script is null terminated cstring.
        /// @return last parsed sub pack.
        tstr<obj> _onWork() override;

    private:
        nint _onTokenEndOfInlineBlock(nint tok);
        node* _onSetElem(runExpr& lhs, const node& rhs);
        node* _onAssignElem(FBOExpr::rule type, node& lhs, node& rhs);
        node* _onConvertAssignElem(runExpr& lhs, node& rhs);
        runExpr* _onRunExpr(node& type, const args& a);

        nint _onScan(YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner);
        void _report(err* new1);
        params _asParams(const args& exprs);

        nbool _onInjectObjSubs(obj& it, defBlock& blk);
        nbool _onInjectCtor(obj& it, defBlock& blk);
        std::vector<std::string> _extractParamTypeNames(const args& types);
        std::string _joinVectorString(const std::vector<std::string>& container) const;
        const node& _onDefArrayType(const narr& items);
        std::vector<std::string> _toDotnames(const node& path);
        void _onCompilationUnit(obj& subpack, defBlock& blk);

    private:
        tokenScan* _mode;
        nbool _isIgnoreWhitespace;
        tokenDispatcher _dispatcher;
        std::vector<ncnt> _indents;
        tstr<obj> _subpack;
        tstr<scope> _filescope;
        std::vector<nint> _states;
        exprMaker _maker;
        smartDedent _dedent;
        srcSupplies _supplies;
    };
}
