#include "parser.hpp"

#include "../ast.hpp"
#include "../ast/ctor.hpp"
#include "../ast/func.hpp"
#include "../ast/genericOrigin.hpp"
#include "../ast/origin.hpp"
#include "../builtin/primitive.hpp"
#include "../frame/thread.hpp"
#include "../type/typeMaker.hpp"
#include "bison/lowparser.hpp"
#include "bison/lowscanner.hpp"
#include "bison/tokenScan.hpp"
#include "worker.inl"
#include <algorithm>
#include <cctype>

namespace nm {

    using std::string;
    template class _nout worker<str, slot>;

    NM_DEF_ME(parser)

    namespace {
        string join(const std::vector<string>& dotnames) {
            string ret;
            for(const string& name: dotnames)
                ret += name;
            return ret;
        }
    }

    nint me::_onScan(YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner) {
        int tok = _mode->onScan(*this, val, loc, scanner);
        if(_isIgnoreWhitespace && tok == NEWLINE) return SCAN_AGAIN;
        _isIgnoreWhitespace = false;

        switch(tok) {
            case SCAN_MODE_NORMAL: setScan<normalScan>(); return SCAN_AGAIN;
            case SCAN_MODE_INDENT: setScan<indentScan>(); return SCAN_AGAIN;
            case SCAN_MODE_INDENT_IGNORE: _isIgnoreWhitespace = true; return SCAN_AGAIN;
            case SCAN_MODE_END: tok = 0; // == yyterminate();
        }

        return tok;
    }

    nint me::onScan(parser& ev, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner, nbool& isBypass) {
        int tok;
        do
            // why do you put redundant _onScan() func?:
            //  because of definately, clang++ bug. when I use continue at switch statement inside
            //  of do-while loop here, it doesn't work like usual 'continue' keyword does, but it
            //  does like 'break'.
            tok = _onScan(val, loc, scanner);
        while(tok == SCAN_AGAIN);

        return tok;
    }

    nint me::onTokenEndOfFile() {
        NM_DI("tokenEvent: onTokenEndOfFile() indents.size()=%d", _indents.size());
        if(_indents.size() <= 1) _dispatcher.add(SCAN_MODE_END);
        else _dispatcher.addFront(onDedent(_indents.front(), SCAN_MODE_END));

        NM_DI("tokenEvent: onEndOfFile: finalize by adding 'NEWLINE', then dispatch end-of-file.");
        return NEWLINE;
    }

    nint me::onTokenColon(nint tok) {
        _dedent.useLater();
        return tok;
    }

    nint me::onTokenComma(nint tok) { return _onTokenEndOfInlineBlock(onIgnoreIndent(tok)); }

    nbool me::onTokenEndOfBraces() {
        if(_strTemplateCnt > 0) return _strTemplateCnt--;
        return false;
    }

    void me::onTokenStartOfStrTemplateBrace() { _strTemplateCnt++; }

    nint me::onTokenLParan(nint tok) {
        _dedent.countUp();
        return onIgnoreIndent(tok);
    }

    nint me::onTokenRParan(nint tok) {
        nint ret = _onTokenEndOfInlineBlock(tok);
        _dedent.countDown();
        return ret;
    }

    node* me::_onDefAssign(const modifier& mod, const node& type, const std::string& name,
        const node& rhs) {
        node* ret =
            _maker.make<defAssignExpr>(name, type, rhs, nulOf<node>(), *_maker.makeSrc(name), mod);
        NM_DI("tokenEvent: onDefAssign(%s, %s, %s, %s)", mod, type, name, rhs);
        return ret;
    }

    nint me::_onTokenEndOfInlineBlock(nint tok) {
        if(!_dedent.canDedent()) return tok;

        NM_DI("tokenEvent: onTokenEndOfInlineBlock: '%c' [%d] use smart dedent!", (char) tok, tok);
        _dispatcher.addFront(tok);
        return _dedent.dedent();
    }

    nint me::onIndent(ncnt col, nint tok) {
        NM_DI("tokenEvent: onIndent(col: %d, tok: %d) indents.size()=%d", col, tok,
            _indents.size());
        _indents.push_back(col);
        _dispatcher.add(tok);
        return INDENT;
    }

    nint me::onDedent(ncnt col, nint tok) {
        NM_DI("tokenEvent: onDedent(col: %d, tok: %d) indents.size()=%d", col, tok,
            _indents.size());

        _indents.pop_back();
        nint now = _indents.back();
        if(now < col) NM_WHEN.exWarn(errCode::WRONG_INDENT_LV, getReport(), col, now, now);

        while(_indents.back() > col) {
            NM_DI("tokenEvent: onDedent: indentlv become %d -> %d", _indents.back(),
                _indents.size() > 1 ? _indents[_indents.size() - 2] : -1);
            _dispatcher.add(DEDENT);
            _indents.pop_back();
            if(_indents.size() <= 0) break;
        }

        _dispatcher.add(tok);
        return DEDENT;
    }

    nint me::onTokenNewLine(nint tok) {
        NM_DI("tokenEvent: onNewLine: _isIgnoreWhitespace=%s, _indents.size()=%d",
            _isIgnoreWhitespace, _indents.size());
        if(!_isIgnoreWhitespace && _indents.size() >= 1) _dispatcher.add(SCAN_MODE_INDENT);
        _dedent.rel();
        return tok;
    }

    nchar me::onScanUnexpected(const nchar* token) {
        NM_WHEN.exErr(errCode::UNEXPECTED_TOK, getReport(), token);
        return token[0];
    }

    nint me::onIgnoreIndent(nint tok) {
        NM_DI("tokenEvent: onIgnoreIndent(%d)", tok);
        _dispatcher.add(SCAN_MODE_INDENT_IGNORE);
        return tok;
    }

    void me::_onEndWork() {
#if NM_IS_DBG
        const point& pt = getArea().start;
        NM_DI("tokenEvent: _onEndWork(%d,%d)", pt.row, pt.col);
#endif
        super::_onEndWork();
    }

    obj* me::onPack(const node& path) {
        std::vector<string> dotnames = _toDotnames(path);
        NM_DI("tokenEvent: onPack(%s)", join(dotnames));

        // pack syntax rule #1:
        //  if there is no specified name of pack, I create an one.
        const std::string& firstName = dotnames[0];
        if(nul(getTask())) _setTask(new slot(manifest(firstName)));
        obj* e = &getTask().getPack();

        const std::string& realName = getTask().getManifest().name;
        if(realName != firstName)
            return error(errCode::PACK_NOT_MATCH, firstName.c_str(), realName.c_str()), e;

        // pack syntax rule #2:
        //  middle name automatically created if not exist.
        //  on interpreting 'mypack' pack, user may uses 'pack' keyword with dotted-name.
        //  for instance,
        //      'pack mypack.component.ui'
        //  in this scenario, mypack instance should be created before. and component sub
        //  pack object can be created in this parsing keyword.
        for(int n = 1; n < dotnames.size(); n++) {
            const std::string& name = dotnames[n];
            origin* sub = &e->sub<origin>(name);
            if(nul(sub)) {
                sub = new origin(typeMaker::make<obj>(name));
                sub->setCallComplete(*new mockNode());
                e->subs().add(name, sub);
                sub->_setOrigin(*sub);
            }
            e = sub;
        }

        return onSubPack(*e);
    }

    obj* me::onPack(const std::string& name) { return onPack(*onGet(name)); }

    obj* me::onSubPack(obj& subpack) {
        NM_DI("tokenEvent: onSubPack()");

        _subpack.bind(subpack);
        return &subpack;
    }

    endExpr* me::onEnd(const blockExpr& blk) {
        endExpr* ret = _maker.make<endExpr>(blk);
        if(_funcs.size() > 0) _funcs.back()->getEnds().add(*ret);

        return ret;
    }

    obj* me::onPack() {
        NM_DI("tokenEvent: onPack()");

        if(nul(getTask())) _setTask(new slot(manifest()));

        auto& newSlot = getTask();
        const std::string& name = getTask().getManifest().name;
        if(name != manifest::DEFAULT_NAME)
            return error(errCode::PACK_NOT_MATCH, manifest::DEFAULT_NAME, name.c_str()),
                   &newSlot.getPack();

        return onSubPack(
            newSlot.getPack()); // this is a default pack containing name as '{default}'.
    }

    blockExpr* me::onBlock(const node& stmt) {
        WHEN_NUL(stmt).exErr(IS_NUL, "stmt").ret(new blockExpr());
        blockExpr* ret = onBlock(*new blockExpr(), stmt);
        NM_DI("tokenEvent: onBlock(%s)", stmt);
        return ret;
    }

    blockExpr* me::onBlock(blockExpr& blk, const node& stmt) {
        WHEN_NUL(blk).exErr(IS_NUL, "blk").ret(_maker.make<blockExpr>());
        WHEN_NUL(stmt).exErr(IS_NUL, "stmt").ret(&blk);
        [[maybe_unused]] func& f = _funcs.size() > 0 ? *_funcs.back() : nulOf<func>();
        str stmtLife(stmt);
        NM_DI("tokenEvent: onBlock(blk, %s) inside of %s func", stmt,
            !nul(f) ? f.getSrc().getName() : "<null>");

        if(!nul(stmt.cast<endExpr>())) return &blk;

        blk.getStmts().add(*stmtLife);
        NM_DI("tokenEvent: onBlock(%d).add(%s)", blk.getStmts().len(), stmt);
        return &blk;
    }

    blockExpr* me::onBlock() {
        NM_DI("tokenEvent: onBlock()");
        return _maker.make<blockExpr>();
    }

    defBlock* me::onDefBlock() {
        NM_DI("tokenEvent: onDefBlock()");
        return new defBlock();
    }

    defBlock* me::onDefBlock(node& stmt) { return onDefBlock(*new defBlock(), stmt); }

    defBlock* me::onDefBlock(defBlock& s, node& stmt) {
        str stmtLife(stmt);

        WHEN_NUL(s).exErr(IS_NUL, "s").ret(new defBlock());
        WHEN_NUL(stmt).exErr(IS_NUL, "stmt").ret(&s);
        NM_DI("tokenEvent: onDefBlock(s, %s)", stmt);

        WHEN(!nul(stmt.cast<endExpr>())).exErr(END_ONLY_BE_IN_A_FUNC).ret(&s);
        defVarExpr& defVar =
            stmt.cast<defVarExpr>() orRet & s.addScope(stmt.getSrc().getName(), *stmtLife);

        // checks whether rhs was primitive type:
        //  if rhs isn't primitive, rhs will be getExpr type.
        //  mockNode will be created
        const baseObj& rhs = defVar.getRight().cast<baseObj>();
        if(!nul(rhs) && rhs.getState() >= PARSED) {
            str new1 = defVar.makeNewOrigin();
            if(new1) return &s.addScope(defVar.getName(), *defVar.makeNewOrigin());
        }

        defVar.setTo(*_maker.make<getExpr>("me"));
        return &s.expand(defVar);
    }

    node* me::onDefProp(const modifier& mod, const std::string& name, const node& rhs) {
        node* ret = _maker.make<defPropExpr>(name, rhs, nulOf<node>(), *_maker.makeSrc(name), mod);
        NM_DI("tokenEvent: onDefProp(%s, %s)", rhs, name);
        return ret;
    }

    node* me::onDefProp(const std::string& name, const node& rhs) {
        return onDefProp(*_makeDefaultModifier(), name, rhs);
    }

    node* me::onDefProp(const node& rhs) { return onDefProp(*_makeDefaultModifier(), rhs); }

    node* me::onDefProp(const modifier& mod, const node& rhs) {
        const getExpr &cast =
            rhs.cast<getExpr>() orRetErr(rhs, SHORT_DEF_VAR_ONLY_ALLOWED_TO_CUSTOM_TYPE).ret(nullptr);
        string newName = cast.getName();
        newName[0] = std::tolower(newName[0]);
        return onDefProp(mod, newName, rhs);
    }

    node* me::onDefAssign(const std::string& name, const node& rhs) {
        return onDefAssign(*_makeDefaultModifier(), name, rhs);
    }

    node* me::onDefAssign(const modifier& mod, const std::string& name, const node& rhs) {
        return _onDefAssign(mod, nulOf<node>(), name, rhs);
    }

    node* me::onDefAssign(const defPropExpr& prop, const node& rhs) {
        WHEN_NUL(prop).exErr(IS_NUL, "prop").ret(nullptr);
        return _onDefAssign(prop.getNewModifier(), prop.getRight(), prop.getName(), rhs);
    }

    node* me::onDefArray(const narr& items) {
        NM_DI("tokenEvent: onDefArray(items.len[%d])", items.len());
        return _maker.make<defArrayExpr>(items);
    }

    node* me::onDefSeq(const node& start, const node& end) {
        NM_DI("tokenEvent: onDefSeq()");
        return _maker.make<defSeqExpr>(start, end);
    }

    void me::onSrcArea(const area& new1) {
        area& prev = _getArea();
        prev = new1;
        ++prev;
        _maker.setRow(prev.start.row).setCol(prev.start.col);
    }

    params me::_asParams(const args& as) {
        params ret;
        for(auto& a: as) {
            tstr<defPropExpr> defProp =
                a.cast<defPropExpr>() orRetErr(PARAM_HAS_VAL).ret(ret);
            ret.add(new param(defProp->getName(), defProp->getRight()));
        }

        return ret;
    }

    func* me::onFuncSignature(const getExpr& access, const node& retType) {
        return onFuncSignature(*new modifier(), access, retType);
    }

    func* me::onFuncSignature(const modifier& mod, const getExpr& access, const node& retType) {
        func* new1 = _maker.birth<func>(access.getName(), mod,
            typeMaker::make<func>(_asParams(access.getArgs()), retType));
        _funcs.push_back(new1);

        NM_DI("tokenEvent: onFuncSignature(%s, access: %s(%d), retType:%s)", mod, access.getName(),
            access.getArgs().len(), retType);
        return new1;
    }

    func* me::onFuncSignature(const modifier& mod, node& it, const node& retType) {
        func* ret = onFuncSignature(mod, onCallAccess(it, *new narr())->cast<getExpr>(), retType);
        NM_DI("tokenEvent: onFuncSignature(%s, it: %s, retType: %s)", mod, it, retType);
        return ret;
    }

    func* me::onFuncSignature(node& it, const node& retType) {
        func* ret = onFuncSignature(onCallAccess(it, *new narr())->cast<getExpr>(), retType);
        NM_DI("tokenEvent: onFuncSignature(it: %s, retType: %s)", it, retType);
        return ret;
    }

    func* me::onAbstractFunc(func& f) {
        _funcs.pop_back();
        f.getBlock().setEval(f.getRet());
        NM_DI("tokenEvent: onAbstractFunc(%s)", f);
        return &f;
    }

    node* me::onFunc(func& f, const blockExpr& blk) {
        NM_DI("tokenEvent: onFunc: func[%s] blk.len()=%d", &f, blk.getStmts().len());

        f.setBlock(blk);
        _funcs.pop_back();
        if(_funcs.size() <= 0) return &f;

        // don't add nested-func in an outer func:
        //  nested func appear in the middle of code, and context is important because they capture
        //  local variables that came before them.
        //  if you put a nested function in the subs() of a func like an ordinary sub-node, some
        //  user may be able to pinpoint this nested function and try to take some action.
        //  and of course the consequences will be dire. this is because you access it without
        //  context.
        func& outer = *_funcs.back();
        node* ret = _maker.make<defNestedFuncExpr>(f);
        NM_I("tokenEvent: onFunc: nested `%s` func in `%s` func", f, outer);
        return ret;
    }

    defNestedFuncExpr* me::onLambda(const narr& params, const node& retType, const blockExpr& blk) {
        defNestedFuncExpr* ret = _maker.make<defNestedFuncExpr>(
            *_maker.birth<func>(func::LAMBDA_NAME, *onModifier(true, false),
                typeMaker::make<func>(_asParams(args(params)), retType), blk));
        NM_DI("tokenEvent: onLambda(params:%d, retType:%s)", params.len(), retType);
        return ret;
    }

    ctor* me::onCtor(const modifier& mod, const narr& a, const blockExpr& blk) {
        ctor* ret = _maker.birth<ctor>(baseObj::CTOR_NAME, mod, _asParams(args(a)), blk);
        NM_DI("tokenEvent: onCtor(%s, args): args.len[%d]", mod, a.len());
        return ret;
    }

    ctor* me::onCtor(const narr& a, const blockExpr& blk) {
        return onCtor(*new modifier(), a, blk);
    }

    ctor* me::onCtor(const modifier& mod, const blockExpr& blk) { return onCtor(mod, narr(), blk); }

    ctor* me::onCtor(const blockExpr& blk) { return onCtor(narr(), blk); }

    narr* me::onParams() {
        NM_DI("tokenEvent: onParams()");
        return new narr();
    }

    narr* me::onParams(const defPropExpr& elem) {
        WHEN_NUL(elem).exErr(IS_NUL, "elem").ret(new narr());
        NM_DI("tokenEvent: onParams(%s %s)", elem.getName(), elem.getRight());
        narr* ret = new narr();
        ret->add(elem);

        return ret;
    }

    narr* me::onParams(narr& it, const defPropExpr& elem) {
        WHEN_NUL(elem).exErr(IS_NUL, "elem").ret(&it);
        NM_DI("tokenEvent: onParams(narr(%d), %s %s)", it.len(), elem.getName(), elem.getRight());
        it.add(elem);

        return &it;
    }

    modifier* me::onModifier(nbool isPublic, nbool isExplicitOverriden) {
        NM_DI("tokenEvent: onModifier(isPublic[%s], isExplicitOverriden[%s])", isPublic,
            isExplicitOverriden);
        return new modifier{isPublic, isExplicitOverriden};
    }

    node* me::onParanthesisAsTuple(narr& tuple) {
        NM_DI("tokenEnvent: onParanthesisAsTuple(%s.size=%d)", (void*) &tuple, tuple.len());

        if(tuple.len() != 1) {
            error(errCode::PARANTHESIS_WAS_TUPLE);
            return new mockNode();
        }

        return &tuple[0];
    }

    args* me::onTuple() {
        NM_DI("tokenEvent: onTuple()");
        return new args();
    }

    args* me::onTuple(const node& elem) {
        args* ret = new args();
        ret->add(elem);

        NM_DI("tokenEvent: onTuple(%s[%s])", elem, &elem);
        return ret;
    }

    args* me::onTuple(args& as, const node& elem) {
        as.add(elem);

        NM_DI("tokenEvent: onTuple(as[%s], %s[%s])", &as, elem, &elem);
        return &as;
    }

    args* me::onFuncCallTuple() {
        NM_DI("tokenEvent: onFuncCallTuple()");
        return new args();
    }

    args* me::onFuncCallTuple(const node& elem) {
        args* ret = new args();
        ret->add(elem);

        NM_DI("tokenEvent: onFuncCallTuple(elem[%s]=%s)", elem, &elem);
        return ret;
    }

    args* me::onFuncCallTuple(args& as, const node& elem) {
        as.add(elem);

        NM_DI("tokenEvent: onFuncCallTuple(as[%s], elem[%s])", &as, elem);
        return &as;
    }

    args* me::onTypeNames(const node& param) {
        auto* ret = new args();
        ret->add(param);

        NM_DI("tokenEvent: onTypeNames(%s)", param);
        return ret;
    }

    args* me::onTypeNames(args& params, const node& param) {
        params.add(param);

        NM_DI("tokenEvent: onTypeNames(len[%d], %s)", params.len(), param);
        return &params;
    }

    obj* me::onDefOrigin(const std::string& name, defBlock& blk) {
        return onDefOrigin(name, narr(), blk);
    }

    obj* me::onDefOrigin(const std::string& name, const narr& a, defBlock& blk) {
        args* newArgs = new args(a);
        std::string argNames = _joinVectorString(_extractParamTypeNames(*newArgs));
        NM_DI("tokenEvent: onDefOrigin(%s, %s, defBlock[%s])", name, argNames, &blk);

        origin& ret = *_maker.birth<origin>(name, typeMaker::make<obj>(name), *_subpack);
        switch(util::checkTypeAttr(name)) {
            case ATTR_COMPLETE: // newArgs.len() can be 0.
                ret.setCallComplete(*_maker.make<runExpr>(ret,
                    *_maker.make<getExpr>(baseObj::CTOR_NAME, *newArgs), *newArgs));
                break;

            case ATTR_INCOMPLETE:
            case ATTR_CONST:
                if(newArgs->len() > 0) NM_WHEN.myExErr(ret, CANT_CALL_COMPLETE_FOR_INCOMPLETE);
                break;

            default: NM_WHEN.myExErr(ret, UNEXPECTED_ATTR);
        }
        _onInjectObjSubs(ret, blk);
        return &ret;
    }

    namespace {
        const std::string& _extractParamTypeName(const node& p) {
            static std::string dummy;
            if(nul(p)) return dummy;
            if(p.isSub<getExpr>()) return ((const getExpr&) p).getName();

            return p.getType().getName();
        }
    }

    std::vector<std::string> me::_extractParamTypeNames(const args& types) {
        std::vector<std::string> ret;
        for(const auto& a: types) {
            // all args should be getExpr instances.
            const std::string& name = _extractParamTypeName(a);
            if(name == "")
                NM_WHEN.exErr(SHOULD_TYPE_PARAM_NAME, getReport(), a.getType()).ret(std::vector<std::string>());

            ret.push_back(name);
        }

        return ret;
    }

    std::vector<string> me::_toDotnames(const node& path) {
        std::vector<string> ret;
        const getExpr* iter = &path.cast<getExpr>();
        if(nul(iter)) {
            error(errCode::PACK_ONLY_ALLOW_VAR_ACCESS);
            return std::vector<string>();
        }

        do {
            ret.push_back(iter->getName());
            const node& next = iter->getMe();
            if(!nul(next) && !next.is<getExpr>()) {
                error(errCode::PACK_ONLY_ALLOW_VAR_ACCESS);
                return std::vector<string>();
            }
            iter = &next.cast<getExpr>();
        } while(iter);
        return ret;
    }

    genericOrigin* me::onDefObjGeneric(const std::string& name, const args& typeParams,
        defBlock& blk) {
        return onDefObjGeneric(name, typeParams, narr(), blk);
    }

    genericOrigin* me::onDefObjGeneric(const std::string& name, const args& typeParams,
        const narr& a, defBlock& blk) {
        args* newArgs = new args(a);
        std::string argNames = _joinVectorString(_extractParamTypeNames(*newArgs));
        NM_DI("tokenEvent: onDefObjGeneric(%s, type.len[%d], args[%s], defBlock[%s]", name,
            typeParams.len(), argNames, &blk);

        nbool isConcerete = util::checkTypeAttr(name) == ATTR_COMPLETE;
        origin& org = *_maker.birth<origin>(name,
            mgdType(name, ttype<obj>::get(), params::make(typeParams), !isConcerete,
                nulOf<node>()));
        if(isConcerete)
            org.setCallComplete(
                *_maker.make<runExpr>(*_maker.make<getGenericExpr>(name, typeParams),
                    *_maker.make<getExpr>(baseObj::CTOR_NAME, *newArgs), *newArgs));
        _onInjectObjSubs(org, blk);
        org._setSubPack(*_subpack);

        std::vector<std::string> paramNames = _extractParamTypeNames(typeParams);
        std::string paramName = _joinVectorString(paramNames);
        return _maker.birth<genericOrigin>(name, org, paramNames);
    }

    std::string me::_joinVectorString(const std::vector<std::string>& container) const {
        std::string ret;
        for(ncnt n = 0; n < container.size(); n++) {
            ret += container[n];
            if(n > 0) ret += ", ";
        }
        return ret;
    }

    void me::onCompilationUnit(obj& subpack) {
        NM_DI("tokenEvent: onCompilationUnit(%s)", &subpack);

        tstr<defBlock> blkLife(new defBlock());
        _onCompilationUnit(subpack, *blkLife);
    }

    void me::onCompilationUnit(obj& subpack, defBlock& blk) {
        NM_DI("tokenEvent: onCompilationUnit(%s, defBlock[%s].expand.len()=%d)", &subpack, &blk,
            blk.getExpands().len());

        _onCompilationUnit(subpack, blk);
    }

    void me::_onCompilationUnit(obj& subpack, defBlock& blk) {
        if(nul(subpack)) return error(errCode::NO_PACK_TRAY), void();

        _onInjectObjSubs(subpack, blk);

        // link system slots:
        subpack.getShares().link(*scope::wrap(thread::get().getSlots()));
        NM_DI("link system slots[%d]: len=%d", thread::get().getSlots().len(),
            subpack.subs().len());

        // at this far, subpack must have at least 1 default ctor created just before:
        NM_DI("tokenEvent: onCompilationUnit: run preconstructor(%d lines)",
            !nul(blk) ? blk.getExpands().len() : 0);
        subpack.run(baseObj::CTOR_NAME); // don't need argument. it's default ctor.
    }

    tstr<modifier> me::_makeDefaultModifier() { return *onModifier(true, false); }

    nbool me::_onInjectObjSubs(obj& it, defBlock& blk) {
        NM_DI("tokenEvent: _onInjectObjSubs(%s, defBlock[%s])", &it, &blk);
        if(nul(it)) return false;

        bicontainable& share = it.getShares().getContainer();
        bicontainable& own = it.getOwns();
        for(auto e = blk.getScope().begin(); e; ++e) {
            // ctor case:
            ctor& c = e.getVal<ctor>();
            if(!nul(c)) c._getType().setRet(*new mockNode(it));

            // shares case:
            // TODO: not only func, but also shared variable.
            func& f = e.getVal<func>();
            if(!nul(f)) f._setOrigin(it);
            bicontainable& con = nul(e.getVal<baseFunc>()) ? own : share;
            con.add(e.getKey(), *e);
        }

        return _onInjectCtor(it, blk);
    }

    nbool me::_onInjectCtor(obj& it, defBlock& blk) {
        const auto& ctors = it.subAll<func>(baseObj::CTOR_NAME);
        nbool hasCopyCtor = !nul(ctors.get<func>([&](const func& f) -> nbool {
            const params& ps = f.getParams();
            if(ps.len() != 1) return false;
            const node& org = ps[0].getOrigin();
            const getExpr& cast = org.cast<getExpr>();
            if(!nul(cast) && cast.getName() == it.getSrc().getName()) return true;
            return &org == &it;
        }));

        // algorithm:
        //  case1: user defined ctor and copy ctor.
        //  case1-1: user defined more than 2 normal ctors.
        //      in this case, there are more than 1 ctor defined.
        //
        //  case2: user defined copy ctor only.
        //      if len of ctors was 1, copyCtor must not be defined.
        //
        //  case3: neither ctor, copyctor has been defined.
        //      hasCtor must be false.
        ncnt len = ctors.len();
        nbool hasCtor = len > 1 || (len == 1 && !hasCopyCtor);
        NM_DI("tokenEvent: _onInjectDefaultCtor(%s, hasCtor=%s, hasCopyCtor=%s)", &it, hasCtor,
            hasCopyCtor);

        // TODO: ctor need to call superclass's ctor.
        if(!hasCtor)
            it.getShares().getContainer().add(baseObj::CTOR_NAME,
                *_maker.make<defaultCtor>(it.getOrigin()));
        if(!hasCopyCtor)
            it.getShares().getContainer().add(baseObj::CTOR_NAME,
                *_maker.make<defaultCopyCtor>(it.getOrigin()));

        // add postpones & common:
        //  if there is no postpones, add() will just return false.
        auto& shares = it.getShares();
        shares.add(baseObj::EXPAND_NAME, _maker.makeExpandFunc(blk));
        shares.add(baseObj::COMMON_NAME, _maker.makeCommonFunc(blk));
        return true;
    }

    retExpr* me::onRet() {
        NM_DI("tokenEvent: onRet()");

        return _maker.make<retExpr>();
    }

    retExpr* me::onRet(node& exp) {
        retExpr* ret = _maker.make<retExpr>(exp);
        NM_DI("tokenEvent: onRet(%s)", exp);
        return ret;
    }

    breakExpr* me::onBreak() {
        NM_DI("tokenEvent: onBreak()");

        return _maker.make<breakExpr>();
    }

    nextExpr* me::onNext() {
        NM_DI("tokenEvent: onNext()");

        return _maker.make<nextExpr>();
    }

    node* me::onGet(const std::string& name) {
        NM_DI("tokenEvent: onGet(%s)", name);
        return _maker.make<getExpr>(name);
    }

    node* me::onGet(const std::string& name, const narr& args) {
        NM_DI("tokenEvent: onGet(%s, %d)", name, args.len());
        return _maker.make<getExpr>(name, args);
    }

    node* me::onGet(node& from, const std::string& name) { return onGet(from, *onGet(name)); }

    node* me::onGet(node& from, node& it) {
        getExpr &cast = it.cast<getExpr>()
                            orRet error(errCode::IDENTIFIER_ONLY, it.getType().getName().c_str()),
                &from;

        cast.setMe(from);
        NM_DI("tokenEvent: onGet(%s, %s)", from, cast.getName());
        return &cast;
    }

    node* me::onGet(const std::string& name, const std::string& name2) {
        return onGet(*onGet(name), *onGet(name2));
    }

    node* me::onCallAccess(node& it, const narr& as) {
        // it can be generic or primitive values. track it, leave as specific errs.
        getExpr &cast = it.cast<getExpr>()
                            orRet error(errCode::IDENTIFIER_ONLY, it.getType().getName().c_str()),
                new getExpr("");

        cast.setArgs(*new args(as));
        return &cast;
    }

    node* me::onGetArray(node& elemType) {
        if(elemType.isSub<nVoid>())
            _report(nerr::newErr(elemType.getSrc().getPos(), ELEM_TYPE_NOT_VOID));
        node* ret = new arr(elemType);
        NM_DI("tokenEvent: onGetArray(%s)", elemType);
        return ret;
    }

    node* me::onGetElem(const node& arr, const node& idx) {
        node* ret = _maker.make<runExpr>(arr,
            *_maker.make<getExpr>(arr, "get", *new args{narr{idx}}), args{narr{idx}});
        NM_DI("tokenEvent: onGetElem(%s, %s)", arr, idx);
        return ret;
    }

    node* me::onGetGeneric(const std::string& orgName, const args& typeParams) {
        NM_DI("tokenEvent: onGetGeneric(%s, params.len[%d])", orgName, typeParams.len());
        return _maker.make<getGenericExpr>(orgName, typeParams);
    }

    runExpr* me::onRunExpr(node& trg, const narr& a) {
        runExpr* ret = _onRunExpr(nulOf<node>(), trg, *new args(a));
        NM_DI("tokenEvent: onRunExpr(%s, narr[%d])", trg, a.len());
        return ret;
    }

    runExpr* me::onRunExpr(node& trg, const args& a) {
        runExpr* ret = _onRunExpr(nulOf<node>(), trg, a);
        NM_DI("tokenEvent: onRunExpr(%s, args[%d])", trg, a.len());
        return ret;
    }

    runExpr* me::onRunExprClosure(node& me, const narr& a) {
        runExpr* ret = _onRunExpr(nulOf<node>(), me, a);
        NM_DI("tokenEvent: onRunExprClosure(%s, args[%d])", me, a.len());
        return ret;
    }

    // @param from  can be expr. so I need to evaluate it through 'as()'.
    runExpr* me::onFillFromOfFuncCall(const node& me, runExpr& to) {
        to.setMe(me);
        NM_DI("tokenEvent: onFillFromOfFuncCall(%s, runExpr[%s])", me, &to);
        return &to;
    }

    node* me::onAssign(node& lhs, node& rhs) {
        // _onSetElem branch:
        //  if user code is 'arr[0] = 1', then it will be interpreted to 'arr.set(0, 1)'
        runExpr& r = lhs.cast<runExpr>();
        if(!nul(r)) {
            auto& name = r TO(getSubj().template cast<getExpr>()) TO(getName());
            if(!nul(name) && name == "get") return _onSetElem(r, rhs);
        }

        node* ret = _maker.make<assignExpr>(lhs, rhs);
        NM_DI("tokenEvent: onAssign(%s, %s)", lhs, rhs);
        return ret;
    }

    node* me::_onSetElem(runExpr& lhs, const node& rhs) {
        //  if user code is 'arr[0] = 1', then it will be interpreted to 'arr.set(0, 1)'
        //  for instance,
        //  AST: before
        //      assignExpr
        //          |-[0]: runExpr (lhs)
        //          |       |-[0]: getExpr: "arr" from 'frame'
        //          |       |-[1]: getExpr: "get" from [0]
        //          |       |       |-[0]: nInt
        //          |       |-[2]: nInt: 0
        //          |-[1]: nInt: 1 (rhs)
        //
        //  after:
        //      runExpr
        //          |-[0]: getExpr: "arr" from 'frame'
        //          |-[1]: getExpr: "set" from [0]
        //          |-[2]: nInt: 0 // --> this was runExpr[2] of lhs.
        //          |-[3]: nInt: 1 (rhs)
        //
        //  conclusion:
        //      1. rename 'get' of 'lhs[1].getExpr.getSubName to 'set'.
        //      2. unbind subArgs of (1).
        //      3. lhs.getArgs().add(rhs).
        NM_DI("tokenEvent:: _onSetElem(%s, %s)", &lhs, &rhs);

        getExpr& subject = lhs.getSubj().cast<getExpr>();
        subject._name = "set";
        subject._args.rel();
        lhs.getArgs().add(rhs);

        return &lhs;
    }

    node* me::_onAssignElem(FBOExpr::rule type, node& lhs, node& rhs) {
        // _onConvertAssignElem branch:
        //  if user code is 'arr[0] = 1', then it will be interpreted to 'arr.set(0, 1)'
        runExpr& cast = lhs.cast<runExpr>();
        if(!nul(cast)) {
            auto& name = cast.getSubj() TO(template cast<getExpr>()) TO(getName());
            if(!nul(name) && name == "get")
                return _onConvertAssignElem(cast, *_maker.make<FBOExpr>(type, lhs, rhs));
        }

        node* ret = onAssign(lhs, *_maker.make<FBOExpr>(type, *(node*) lhs.clone(), rhs));
        NM_DI("tokenEvent: onFBOAssign(%s, %s)", lhs, rhs);
        return ret;
    }

    node* me::_onConvertAssignElem(runExpr& lhs, node& rhs) {
        // if user code is 'arr[0] += 1' then it will be interpreted to 'arr.set(0, arr.get(0) + 1)
        // for instance,
        // AST: before
        //  lhs: arr.get(0)
        //      runExpr
        //          |-[0]: getExpr: "arr" from 'frame'
        //          |-[1]: getExpr: "get" from [0]
        //          |-[2]: nInt: 0
        //  rhs: 1
        //      nInt: 1
        //
        // after: arr.set(0, arr.get(0) + 1)
        //  runExpr
        //      |-[0]: getExpr: "arr" from 'frame
        //      |-[1]: getExpr: "set" from [0]
        //      |-[2]: nInt: 0
        //      |-[3]: FBOExpr(ADD) (rhs of this func)
        //              |-[0]: runExpr (lhs of this func)
        //              |         |-[0]: getExpr: "arr" from 'frame'
        //              |         |-[1]: getExpr: "get" from [0]
        //              |         |-[2]: nInt: 0
        //              |-[1]: nInt: 1
        //
        // conclusion:
        //  1. deep clone lhs and set its[1].subName to 'set'
        //  2. (1).getArgs.add(rhs)
        runExpr& setter = *(runExpr*) lhs.clone();
        getExpr& newSubj = *(getExpr*) setter.getSubj().clone();
        newSubj._name = "set";
        newSubj._args.rel();
        setter._subject.bind(newSubj);
        setter.getArgs().add(rhs);
        NM_DI("tokenEvent: _onConvertAssignElem(%s, %s)", lhs, rhs);
        return &setter;
    }

    runExpr* me::_onRunExpr(node& me, node& trg, const args& a) {
        runExpr* ret = _maker.make<runExpr>(me, trg, a);
        getExpr& cast = trg.cast<getExpr>();
        if(!nul(cast) && !cast.isSub<getGenericExpr>()) cast.setArgs(a);
        return ret;
    }

    node* me::onAddAssign(node& lhs, node& rhs) {
        node* ret = _onAssignElem(FBOExpr::ADD, lhs, rhs);
        NM_DI("tokenEvent: onAddAssign(%s, %s)", lhs, rhs);
        return ret;
    }

    node* me::onSubAssign(node& lhs, node& rhs) {
        node* ret = _onAssignElem(FBOExpr::SUB, lhs, rhs);
        NM_DI("tokenEvent: onSubAssign(%s, %s)", lhs, rhs);
        return ret;
    }

    node* me::onMulAssign(node& lhs, node& rhs) {
        node* ret = _onAssignElem(FBOExpr::MUL, lhs, rhs);
        NM_DI("tokenEvent: onMulAssign(%s, %s)", lhs, rhs);
        return ret;
    }

    node* me::onDivAssign(node& lhs, node& rhs) {
        node* ret = _onAssignElem(FBOExpr::DIV, lhs, rhs);
        NM_DI("tokenEvent: onDivAssign(%s, %s)", lhs, rhs);
        return ret;
    }

    node* me::onModAssign(node& lhs, node& rhs) {
        node* ret = _onAssignElem(FBOExpr::MOD, lhs, rhs);
        NM_DI("tokenEvent: onModAssign(%s, %s)", lhs, rhs);
        return ret;
    }

    isExpr* me::onIs(const node& me, const node& to) {
        isExpr* ret = _maker.make<isExpr>(me, to);
        NM_DI("tokenEvent: onIs(%s, %s)", me, to);
        return ret;
    }

    asExpr* me::onAs(const node& me, const node& as) {
        asExpr* ret = _maker.make<asExpr>(me, as);
        NM_DI("tokenEvent: onAs(%s, %s)", me, as);
        return ret;
    }

    FBOExpr* me::onUnaryMinus(const node& it) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::MUL, it, *new nInt(-1));
        NM_DI("tokenEvent: onUnaryMinus(%s)", it);
        return ret;
    }

    node* me::onUnaryDoublePlus(node& it) {
        node* ret = onAssign(it, *_maker.make<FBOExpr>(FBOExpr::ADD, it, *new nInt(1)));
        NM_DI("tokenEvent: onUnaryDoublePlus(%s)", it);
        return ret;
    }

    node* me::onUnaryDoubleMinus(node& it) {
        node* ret = onAssign(it, *_maker.make<FBOExpr>(FBOExpr::SUB, it, *new nInt(1)));
        NM_DI("tokenEvent: onUnaryDoubleMinus(%s)", it);
        return ret;
    }

    FUOExpr* me::onUnaryPostfixDoublePlus(const node& it) {
        FUOExpr* ret = _maker.make<FUOExpr>(FUOExpr::POSTFIX_DOUBLE_PLUS, it);
        NM_DI("tokenEvent: onUnaryPostfixDoublePlus(%s)", it);
        return ret;
    }

    FUOExpr* me::onUnaryPostfixDoubleMinus(const node& it) {
        FUOExpr* ret = _maker.make<FUOExpr>(FUOExpr::POSTFIX_DOUBLE_MINUS, it);
        NM_DI("tokenEvent: onUnaryPostfixDoubleMinus(%s)", it);
        return ret;
    }

    FBOExpr* me::onUnaryNot(const node& it) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::EQ, it, *new nBool(false));
        NM_DI("tokenEvent: onUnaryNot(%s)", it);
        return ret;
    }

    FUOExpr* me::onUnaryBitwiseNot(const node& it) {
        FUOExpr* ret = _maker.make<FUOExpr>(FUOExpr::BITWISE_NOT, it);
        NM_DI("tokenEvent: onUnaryBitwiseNot(%s)", it);
        return ret;
    }

    FBOExpr* me::onAdd(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::ADD, lhs, rhs);
        NM_DI("tokenEvent: onAdd(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onSub(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::SUB, lhs, rhs);
        NM_DI("tokenEvent: onSub(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onMul(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::MUL, lhs, rhs);
        NM_DI("tokenEvent: onMul(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onDiv(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::DIV, lhs, rhs);
        NM_DI("tokenEvent: onDiv(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onMod(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::MOD, lhs, rhs);
        NM_DI("tokenEvent: onMod(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onBitwiseAnd(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::BITWISE_AND, lhs, rhs);
        NM_DI("tokenEvent: onBitwiseAnd(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onBitwiseOr(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::BITWISE_OR, lhs, rhs);
        NM_DI("tokenEvent: onBitwiseOr(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onBitwiseXor(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::BITWISE_XOR, lhs, rhs);
        NM_DI("tokenEvent: onBitwiseXor(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onLShift(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::LSHIFT, lhs, rhs);
        NM_DI("tokenEvent: LShift(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onRShift(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::RSHIFT, lhs, rhs);
        NM_DI("tokenEvent: RShift(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onGt(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::GT, lhs, rhs);
        NM_DI("tokenEvent: onGt(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onLt(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::LT, lhs, rhs);
        NM_DI("tokenEvent: onLt(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onGe(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::GE, lhs, rhs);
        NM_DI("tokenEvent: onGe(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onLe(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::LE, lhs, rhs);
        NM_DI("tokenEvent: onLe(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onEq(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::EQ, lhs, rhs);
        NM_DI("tokenEvent: onEq(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onNe(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::NE, lhs, rhs);
        NM_DI("tokenEvent: onNe(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onAnd(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::AND, lhs, rhs);
        NM_DI("tokenEvent: onAnd(%s, %s)", lhs, rhs);
        return ret;
    }

    FBOExpr* me::onOr(const node& lhs, const node& rhs) {
        FBOExpr* ret = _maker.make<FBOExpr>(FBOExpr::OR, lhs, rhs);
        NM_DI("tokenEvent: onOr(%s, %s)", lhs, rhs);
        return ret;
    }

    node* me::onFor(const std::string& iterName, const node& expr, const blockExpr& blk) {
        node* ret = _maker.make<forExpr>(iterName, expr, blk);
        NM_DI("tokenEvent: onFor(%s, %s)", iterName, expr);
        return ret;
    }

    node* me::onWhile(const node& condition, const blockExpr& blk) {
        node* ret = _maker.make<whileExpr>(condition, blk);
        NM_DI("tokenEvent: onWhile(%s)", condition);
        return ret;
    }

    ifExpr* me::onIf(const node& condition, const blockExpr& then) {
        NM_DI("tokenEvent: onIf(then)");
        return _maker.make<ifExpr>(condition, then);
    }

    ifExpr* me::onIf(const node& condition, const blockExpr& then, const blockExpr& elseBlk) {
        NM_DI("tokenEvent: onIf(condition, then, elseBlk)");
        ifExpr* ret = onIf(condition, then);

        ret->setElse(elseBlk);
        return ret;
    }

    ifExpr* me::onIf(const node& condition, const blockExpr& then, const ifExpr& elseIf) {
        NM_DI("tokenEvent: onIf(condition, then, elseIf)");
        ifExpr* ret = onIf(condition, then);
        ret->setElse(*new blockExpr(elseIf));
        return ret;
    }

    runExpr* me::onIn(const node& it, const node& container) {
        runExpr* ret = _maker.make<runExpr>(container, *_maker.make<getExpr>("in"),
            args{nulOf<baseObj>(), it});
        NM_DI("tokenEvent: onIn(%s, %s)", it, container);
        return ret;
    }

    void me::onParseErr(const std::string& msg, const nchar* symbolName) {
        NM_WHEN.exErr(SYNTAX_ERR, getReport(), msg, symbolName);
    }

    me::parser(): _mode(nullptr), _isIgnoreWhitespace(false) { rel(); }

    node& me::getSubPack() {
        return *_subpack; // TODO: can I remove subpack variable?
    }

    srcSupplies& me::getSrcSupplies() { return _supplies; }

    me& me::addSupply(const srcSupply& new1) {
        _supplies.add(new1);
        return *this;
    }

    me& me::relSupplies() {
        _supplies.rel();
        return *this;
    }

    tokenDispatcher& me::getDispatcher() { return _dispatcher; }

    std::vector<ncnt>& me::getIndents() { return _indents; }

    nbool me::isInit() const { return _mode; }

    void me::rel() {
        super::rel();

        _states.clear();
        _states.push_back(0); // 0 for default state
        _dedent.rel();
        _supplies.rel();
        _maker.rel();
        _prepare();
    }

    void me::_prepare() {
        super::_prepare();

        _mode = nullptr;
        _subpack.rel();
        _isIgnoreWhitespace = false;
        _dispatcher.rel();
        _indents.clear();
        _maker.setRow(0).setCol(0);
        _strTemplateCnt = 0;
    }

    int me::pushState(int newState) {
        NM_I("push state %d -> %d", _states.back(), newState);
        _states.push_back(newState);
        return _states.back();
    }

    int me::popState() {
        int previous = _states.back();
        _states.pop_back();
        NM_I("pop state %d -> %d", previous, _states.back());
        return _states.back();
    }

    str me::_onWork() {
        const auto& supplies = getSrcSupplies();
        if(supplies.isEmpty()) return error(NO_SRC), tstr<obj>();

        NM_I("parse starts: %d src will be supplied.", supplies.len());
        for(const auto& supply: supplies) {
            _prepare();

            yyscan_t scanner;
            yylex_init_extra(this, &scanner);

            YY_BUFFER_STATE bufState = (YY_BUFFER_STATE) supply.onSupplySrc(*this, scanner);
            if(!bufState) {
                error(errCode::IS_NUL, "bufState");
                return tstr<obj>();
            }

            // fix Flex Bug here:
            //  when yy_scan_string get called, it returns bufState after malloc it.
            //  but some variables wasn't initialized. yy_bs_lineno(used to calculate
            //  current cursor position) is one of them.
            bufState->yy_bs_lineno = bufState->yy_bs_column = 0;
            yy_switch_to_buffer(bufState, scanner);

#if YYDEBUG
            // yyset_debug(1, scanner); // For Flex (no longer a global, but rather a member of)
            // yydebug = 1;             // For Bison (still global, even in a reentrant)
#endif

            int res = yyparse(scanner);
            if(res) {
                warn(errCode::PARSING_HAS_ERR, res);
                return tstr<obj>();
            }

            yy_delete_buffer(bufState, scanner);
            yylex_destroy(scanner);
        }

        return getSubPack();
    }

    void me::_report(baseErr* new1) { getReport() TO(add(new1)); }

    exprMaker& me::_getMaker() { return _maker; }
} // namespace nm
