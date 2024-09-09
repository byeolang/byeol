#include "parser.hpp"

#include "../ast.hpp"
#include "../ast/ctor.hpp"
#include "../ast/func.hpp"
#include "../ast/genericOrigin.hpp"
#include "../ast/origin.hpp"
#include "../builtin/primitive.hpp"
#include "../frame/thread.hpp"
#include "../type/mgdType.hpp"
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
        _dedent.setEnable(true);
        return tok;
    }

    nint me::onTokenComma(nint tok) {
        _dedent.countDown();
        return _onTokenEndOfInlineBlock(onIgnoreIndent(tok));
    }

    nint me::onTokenLParan(nint tok) {
        _dedent.countUp();
        return onIgnoreIndent(tok);
    }

    nint me::onTokenRParan(nint tok) {
        _dedent.countDown();
        return _onTokenEndOfInlineBlock(tok);
    }

    nint me::_onTokenEndOfInlineBlock(nint tok) {
        if(!_dedent.canDedent()) return tok;

        NM_DI("tokenEvent: onTokenEndOfInlineBlock: '%c' [%d] use smart dedent!", (char) tok, tok);
        _dispatcher.addFront(tok);
        _dedent.setEnable(false);
        return NEWLINE;
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
        if(now < col) posWarn(errCode::WRONG_INDENT_LV, col, now, now);

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
        _dedent.setEnable(false);
        return tok;
    }

    nchar me::onScanUnexpected(const nchar* token) {
        posError(errCode::UNEXPECTED_TOK, token);
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
                sub = new origin(mgdType::make(name));
                sub->setCallComplete(*new mockNode());
                e->subs().add(name, sub);
                sub->_setOrigin(*sub);
            }
            e = sub;
        }

        return onSubPack(*e);
    }

    obj* me::onPack(const std::string& name) {
        return onPack(*onGet(name));
    }

    obj* me::onSubPack(obj& subpack) {
        NM_DI("tokenEvent: onSubPack()");
        _subpack.bind(subpack);
        return &subpack;
    }

    endExpr* me::onEnd(const blockExpr& blk) {
        endExpr* ret = _maker.make<endExpr>(blk);
        if(_func) _func->getEnds().add(*ret);

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
        NM_DI("tokenEvent: onBlock(%s) insideOf %s func", stmt, _func ? _func->getSrc().getName() : "<null>");
        if(!nul(stmt.cast<endExpr>())) return _maker.make<blockExpr>();
        return _maker.make<blockExpr>(stmt);
    }

    blockExpr* me::onBlock(blockExpr& blk, const node& stmt) {
        NM_DI("tokenEvent: onBlock(blk, %s) inside of %s func", stmt, _func ? _func->getSrc().getName() : "<null>");
        if(nul(blk)) return posError(errCode::IS_NUL, "blk"), _maker.make<blockExpr>();
        if(!nul(stmt.cast<endExpr>())) return &blk;

        blk.getStmts().add(stmt);
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

    defBlock* me::onDefBlock(node& stmt) {
        NM_DI("tokenEvent: onDefBlock(%s)", stmt);
        return onDefBlock(*new defBlock(), stmt);
    }

    defBlock* me::onDefBlock(defBlock& s, node& stmt) {
        NM_DI("tokenEvent: onDefBlock(s, %s)", stmt);
        if(nul(s)) return posError(errCode::IS_NUL, "s"), new defBlock();

        if(!nul(stmt.cast<endExpr>())) return posError(errCode::END_ONLY_BE_IN_A_FUNC), &s;
        defVarExpr& defVar = stmt.cast<defVarExpr>();
        if(nul(defVar)) return &s.addScope(stmt.getSrc().getName(), stmt);

        // checks whether rhs was primitive type:
        //  if rhs isn't primitive, rhs will be getExpr type.
        //  mockNode will be created
        const baseObj& rhs = defVar.getRight().cast<baseObj>();
        if(!nul(rhs) && rhs.getState() >= PARSED)
            return &s.addScope(defVar.getName(), *defVar.makeNew());

        defVar.setTo(*_maker.make<getExpr>("me"));
        return &s.expand(defVar);
    }

    node* me::onDefProp(const modifier& mod, const std::string& name, const node& rhs) {
        NM_DI("tokenEvent: onDefProp(%s, %s)", rhs, name);

        return _maker.make<defPropExpr>(name, rhs, nulOf<node>(), *_maker.makeSrc(name), mod);
    }

    node* me::onDefProp(const std::string& name, const node& rhs) {
        return onDefProp(*onModifier(true, false), name, rhs);
    }

    node* me::onDefAssign(const modifier& mod, const std::string& name, const node& rhs) {
        NM_DI("tokenEvent: onDefAssign(%s, %s, %s)", mod, rhs, name);

        return _maker.make<defAssignExpr>(name, rhs, nulOf<node>(), *_maker.makeSrc(name), mod);
    }

    node* me::onDefAssign(const std::string& name, const node& rhs) {
        return onDefAssign(*onModifier(true, false), name, rhs);
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
            tstr<defPropExpr> defProp(a.cast<defPropExpr>());
            if(!defProp) return posError(errCode::PARAM_HAS_VAL), ret;

            ret.add(new param(defProp->getName(), defProp->getRight()));
        }

        return ret;
    }

    func* me::onAbstractFunc(const getExpr& access, const node& retType) {
        return onAbstractFunc(*new modifier(), access, retType);
    }

    func* me::onAbstractFunc(const modifier& mod, const getExpr& access, const node& retType) {
        NM_DI("tokenEvent: onAbstractFunc(%s, access: %s(%d), retType:%s)", mod, access.getName(),
            access.getArgs().len(), retType);

        _func.bind(_maker.birth<func>(access.getName(), mod, _asParams(access.getArgs()), retType));
        return &_func.get();
    }

    func* me::onAbstractFunc(const modifier& mod, node& it, const node& retType) {
        NM_DI("tokenEvent: onAbstractFunc(%s, it: %s, retType: %s)", mod, it, retType);
        return onAbstractFunc(mod, onCallAccess(it, *new narr())->cast<getExpr>(), retType);
    }

    func* me::onAbstractFunc(node& it, const node& retType) {
        NM_DI("tokenEvent: onAbstractFunc(it: %s, retType: %s)", it, retType);
        return onAbstractFunc(onCallAccess(it, *new narr())->cast<getExpr>(), retType);
    }

    func* me::onFunc(func& f, const blockExpr& blk) {
        NM_DI("tokenEvent: onFunc: func[%s] blk.len()=%d", (void*) &f, blk.getStmts().len());

        f.setBlock(blk);
        onEndFunc();
        return &f;
    }

    ctor* me::onCtor(const modifier& mod, const narr& a, const blockExpr& blk) {
        NM_DI("tokenEvent: onCtor(%s, args): args.len[%d]", mod, a.len());

        return _maker.birth<ctor>(baseObj::CTOR_NAME, mod, _asParams(args(a)), blk);
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
        NM_DI("tokenEvent: onParams(%s %s)", elem.getName(), elem.getRight());
        narr* ret = new narr();
        ret->add(elem);
        return ret;
    }

    narr* me::onParams(narr& it, const defPropExpr& elem) {
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
        NM_DI("tokenEnvent: onParanthesisAsTuple(%s.size=%d)", &tuple, tuple.len());

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
        NM_DI("tokenEvent: onTuple(%s[%s])", elem, &elem);
        args* ret = new args();
        ret->add(elem);
        return ret;
    }

    args* me::onTuple(args& as, const node& elem) {
        NM_DI("tokenEvent: onTuple(as[%s], %s[%s])", &as, elem, &elem);
        as.add(elem);
        return &as;
    }

    args* me::onFuncCallTuple() {
        NM_DI("tokenEvent: onFuncCallTuple()");
        return new args();
    }

    args* me::onFuncCallTuple(const node& elem) {
        NM_DI("tokenEvent: onFuncCallTuple(elem[%s]=%s)", elem, &elem);
        args* ret = new args();
        ret->add(elem);
        return ret;
    }

    args* me::onFuncCallTuple(args& as, const node& elem) {
        NM_DI("tokenEvent: onFuncCallTuple(as[%s], elem[%s])", &as, elem);
        as.add(elem);
        return &as;
    }

    args* me::onTypeNames(const node& param) {
        NM_DI("tokenEvent: onTypeNames(%s)", param);
        auto* ret = new args();
        ret->add(param);
        return ret;
    }

    args* me::onTypeNames(args& params, const node& param) {
        NM_DI("tokenEvent: onTypeNames(len[%d], %s)", params.len(), param);
        params.add(param);
        return &params;
    }

    obj* me::onDefOrigin(const std::string& name, defBlock& blk) {
        return onDefOrigin(name, narr(), blk);
    }

    obj* me::onDefOrigin(const std::string& name, const narr& a, defBlock& blk) {
        args* newArgs = new args(a);
        std::string argNames = _joinVectorString(_extractParamTypeNames(*newArgs));
        NM_DI("tokenEvent: onDefOrigin(%s, %s, defBlock[%s])", name, argNames, &blk);

        origin& ret = *_maker.birth<origin>(name, mgdType::make(name), *_subpack);
        if(util::checkTypeAttr(name) == COMPLETE_OBJ)
            ret.setCallComplete(*_maker.make<runExpr>(ret,
                *_maker.make<getExpr>(baseObj::CTOR_NAME, *newArgs), *newArgs));
        _onInjectObjSubs(ret, blk);
        return &ret;
    }

    std::vector<std::string> me::_extractParamTypeNames(const args& types) {
        std::vector<std::string> ret;
        for(const auto& a: types) {
            // all args should be getExpr instances.
            const getExpr& cast = a.cast<getExpr>();
            if(nul(cast))
                return posError(errCode::SHOULD_TYPE_PARAM_NAME, a.getType().getName().c_str()),
                       std::vector<std::string>();

            ret.push_back(cast.getName());
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

        origin& org = *_maker.birth<origin>(name, mgdType(name, ttype<obj>::get(), params::make(typeParams)));
        if(util::checkTypeAttr(name) == COMPLETE_OBJ)
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
        subpack.getShares().link(thread::get().getSlots());
        NM_DI("link system slots[%d]: len=%d", thread::get().getSlots().len(),
            subpack.subs().len());

        // at this far, subpack must have at least 1 default ctor created just before:
        NM_DI("tokenEvent: onCompilationUnit: run preconstructor(%d lines)",
            !nul(blk) ? blk.getExpands().len() : 0);
        subpack.run(baseObj::CTOR_NAME); // don't need argument. it's default ctor.
    }

    nbool me::_onInjectObjSubs(obj& it, defBlock& blk) {
        NM_DI("tokenEvent: _onInjectObjSubs(%s, defBlock[%s])", it, &blk);
        if(nul(it)) return false;

        bicontainable& share = it.getShares().getContainer();
        bicontainable& own = it.getOwns();
        for(auto e = blk.getScope().begin(); e; ++e) {
            // ctor case:
            ctor& c = e.getVal<ctor>();
            if(!nul(c)) c.setRet(*new mockNode(it));

            // shares case:
            // TODO: not only func, but also shared variable.
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
        NM_DI("tokenEvent: _onInjectDefaultCtor(%s, hasCtor=%s, hasCopyCtor)", it, hasCtor,
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
        auto& subs = it.subs();
        subs.add(baseObj::EXPAND_NAME, _maker.makeExpandFunc(blk));
        subs.add(baseObj::COMMON_NAME, _maker.makeCommonFunc(blk));
        return true;
    }

    retExpr* me::onRet() {
        NM_DI("tokenEvent: onRet()");

        return _maker.make<retExpr>();
    }

    retExpr* me::onRet(node& exp) {
        NM_DI("tokenEvent: onRet(%s)", exp);

        return _maker.make<retExpr>(exp);
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
        getExpr& cast = it.cast<getExpr>();
        if(nul(cast)) {
            error(errCode::IDENTIFIER_ONLY, it.getType().getName().c_str());
            return &from;
        }

        NM_DI("tokenEvent: onGet(%s, %s)", from, cast.getName());
        cast.setMe(from);
        return &cast;
    }

    node* me::onGet(const std::string& name, const std::string& name2) {
        return onGet(*onGet(name), *onGet(name2));
    }

    node* me::onCallAccess(node& it, const narr& as) {
        getExpr& cast = it.cast<getExpr>();
        if(nul(cast)) {
            // it can be generic or primitive values. track it, leave as specific errs.
            error(errCode::IDENTIFIER_ONLY, it.getType().getName().c_str());
            return new getExpr("");
        }

        cast.setArgs(*new args(as));
        return &cast;
    }

    node* me::onGetArray(node& elemType) {
        NM_DI("tokenEvent: onGetArray(%s)", elemType);
        if(elemType.isSub<nVoid>()) error(elemType.getSrc().getPos(), errCode::ELEM_TYPE_NOT_VOID);
        return new arr(elemType);
    }

    node* me::onGetElem(const node& arr, const node& idx) {
        NM_DI("tokenEvent: onGetElem(%s, %s)", arr, idx);

        return _maker.make<runExpr>(arr, *_maker.make<getExpr>(arr, "get", *new args{narr{idx}}),
            args{narr{idx}});
    }

    node* me::onGetGeneric(const std::string& orgName, const args& typeParams) {
        NM_DI("tokenEvent: onGetGeneric(%s, params.len[%d])", orgName, typeParams.len());
        return _maker.make<getGenericExpr>(orgName, typeParams);
    }

    runExpr* me::onRunExpr(node& trg, const narr& a) {
        NM_DI("tokenEvent: onRunExpr(%s, narr[%d])", trg, a.len());
        return _onRunExpr(trg, *new args(a));
    }

    runExpr* me::onRunExpr(node& trg, const args& a) {
        NM_DI("tokenEvent: onRunExpr(%s, args[%d])", trg, a.len());
        return _onRunExpr(trg, a);
    }

    // @param from  can be expr. so I need to evaluate it through 'as()'.
    runExpr* me::onFillFromOfFuncCall(const node& me, runExpr& to) {
        NM_DI("tokenEvent: onFillFromOfFuncCall(%s, runExpr[%s])", me, &to);

        to.setMe(me);
        return &to;
    }

    node* me::onAssign(node& lhs, node& rhs) {
        NM_DI("tokenEvent: onAssign(%s, %s)", lhs, rhs);
        // _onSetElem branch:
        //  if user code is 'arr[0] = 1', then it will be interpreted to 'arr.set(0, 1)'
        runExpr& r = lhs.cast<runExpr>();
        if(!nul(r)) {
            auto& name = safeGet(r, getSubj().cast<getExpr>(), getName());
            if(!nul(name) && name == "get") return _onSetElem(r, rhs);
        }

        return _maker.make<assignExpr>(lhs, rhs);
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
        NM_DI("tokenEvent:: _onSetElem(%s, %s)", lhs, rhs);

        getExpr& subject = lhs.getSubj().cast<getExpr>();
        subject._name = "set";
        subject._args.rel();
        lhs.getArgs().add(rhs);

        return &lhs;
    }

    node* me::_onAssignElem(FBOExpr::rule type, node& lhs, node& rhs) {
        NM_DI("tokenEvent: onFBOAssign(%s, %s)", lhs, rhs);

        // _onConvertAssignElem branch:
        //  if user code is 'arr[0] = 1', then it will be interpreted to 'arr.set(0, 1)'
        runExpr& cast = lhs.cast<runExpr>();
        if(!nul(cast)) {
            auto& name = safeGet(cast.getSubj(), cast<getExpr>(), getName());
            if(!nul(name) && name == "get")
                return _onConvertAssignElem(cast, *_maker.make<FBOExpr>(type, lhs, rhs));
        }

        return onAssign(lhs, *_maker.make<FBOExpr>(type, *(node*) lhs.clone(), rhs));
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
        NM_DI("tokenEvent: _onConvertAssignElem(%s, %s)", lhs, rhs);

        runExpr& setter = *(runExpr*) lhs.clone();
        getExpr& newSubj = *(getExpr*) setter.getSubj().clone();
        newSubj._name = "set";
        newSubj._args.rel();
        setter._subject.bind(newSubj);
        setter.getArgs().add(rhs);
        return &setter;
    }

    runExpr* me::_onRunExpr(node& trg, const args& a) {
        runExpr* ret = _maker.make<runExpr>(nulOf<node>(), trg, a);
        getExpr& cast = trg.cast<getExpr>();
        if(!nul(cast) && !cast.isSub<getGenericExpr>()) cast.setArgs(a);
        return ret;
    }

    node* me::onAddAssign(node& lhs, node& rhs) {
        NM_DI("tokenEvent: onAddAssign(%s, %s)", lhs, rhs);
        return _onAssignElem(FBOExpr::ADD, lhs, rhs);
    }

    node* me::onSubAssign(node& lhs, node& rhs) {
        NM_DI("tokenEvent: onSubAssign(%s, %s)", lhs, rhs);
        return _onAssignElem(FBOExpr::SUB, lhs, rhs);
    }

    node* me::onMulAssign(node& lhs, node& rhs) {
        NM_DI("tokenEvent: onMulAssign(%s, %s)", lhs, rhs);
        return _onAssignElem(FBOExpr::MUL, lhs, rhs);
    }

    node* me::onDivAssign(node& lhs, node& rhs) {
        NM_DI("tokenEvent: onDivAssign(%s, %s)", lhs, rhs);
        return _onAssignElem(FBOExpr::DIV, lhs, rhs);
    }

    node* me::onModAssign(node& lhs, node& rhs) {
        NM_DI("tokenEvent: onModAssign(%s, %s)", lhs, rhs);
        return _onAssignElem(FBOExpr::MOD, lhs, rhs);
    }

    isExpr* me::onIs(const node& me, const node& to) {
        NM_DI("tokenEvent: onIs(%s, %s)", me, to);

        return _maker.make<isExpr>(me, to);
    }

    asExpr* me::onAs(const node& me, const node& as) {
        NM_DI("tokenEvent: onAs(%s, %s)", me, as);

        return _maker.make<asExpr>(me, as);
    }

    FBOExpr* me::onUnaryMinus(const node& it) {
        NM_DI("tokenEvent: onUnaryMinus(%s)", it);

        return _maker.make<FBOExpr>(FBOExpr::MUL, it, *new nInt(-1));
    }

    node* me::onUnaryDoublePlus(node& it) {
        NM_DI("tokenEvent: onUnaryDoublePlus(%s)", it);

        return onAssign(it, *_maker.make<FBOExpr>(FBOExpr::ADD, it, *new nInt(1)));
    }

    node* me::onUnaryDoubleMinus(node& it) {
        NM_DI("tokenEvent: onUnaryDoubleMinus(%s)", it);

        return onAssign(it, *_maker.make<FBOExpr>(FBOExpr::SUB, it, *new nInt(1)));
    }

    FUOExpr* me::onUnaryPostfixDoublePlus(const node& it) {
        NM_DI("tokenEvent: onUnaryPostfixDoublePlus(%s)", it);

        return _maker.make<FUOExpr>(FUOExpr::POSTFIX_DOUBLE_PLUS, it);
    }

    FUOExpr* me::onUnaryPostfixDoubleMinus(const node& it) {
        NM_DI("tokenEvent: onUnaryPostfixDoubleMinus(%s)", it);

        return _maker.make<FUOExpr>(FUOExpr::POSTFIX_DOUBLE_MINUS, it);
    }

    FBOExpr* me::onUnaryNot(const node& it) {
        NM_DI("tokenEvent: onUnaryNot(%s)", it);

        return _maker.make<FBOExpr>(FBOExpr::EQ, it, *new nBool(false));
    }

    FUOExpr* me::onUnaryBitwiseNot(const node& it) {
        NM_DI("tokenEvent: onUnaryBitwiseNot(%s)", it);

        return _maker.make<FUOExpr>(FUOExpr::BITWISE_NOT, it);
    }

    FBOExpr* me::onAdd(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onAdd(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::ADD, lhs, rhs);
    }

    FBOExpr* me::onSub(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onSub(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::SUB, lhs, rhs);
    }

    FBOExpr* me::onMul(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onMul(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::MUL, lhs, rhs);
    }

    FBOExpr* me::onDiv(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onDiv(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::DIV, lhs, rhs);
    }

    FBOExpr* me::onMod(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onMod(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::MOD, lhs, rhs);
    }

    FBOExpr* me::onBitwiseAnd(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onBitwiseAnd(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::BITWISE_AND, lhs, rhs);
    }

    FBOExpr* me::onBitwiseOr(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onBitwiseOr(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::BITWISE_OR, lhs, rhs);
    }

    FBOExpr* me::onBitwiseXor(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onBitwiseXor(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::BITWISE_XOR, lhs, rhs);
    }

    FBOExpr* me::onLShift(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: LShift(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::LSHIFT, lhs, rhs);
    }

    FBOExpr* me::onRShift(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: RShift(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::RSHIFT, lhs, rhs);
    }

    FBOExpr* me::onGt(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onGt(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::GT, lhs, rhs);
    }

    FBOExpr* me::onLt(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onLt(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::LT, lhs, rhs);
    }

    FBOExpr* me::onGe(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onGe(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::GE, lhs, rhs);
    }

    FBOExpr* me::onLe(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onLe(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::LE, lhs, rhs);
    }

    FBOExpr* me::onEq(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onEq(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::EQ, lhs, rhs);
    }

    FBOExpr* me::onNe(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onNe(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::NE, lhs, rhs);
    }

    FBOExpr* me::onAnd(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onAnd(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::AND, lhs, rhs);
    }

    FBOExpr* me::onOr(const node& lhs, const node& rhs) {
        NM_DI("tokenEvent: onOr(%s, %s)", lhs, rhs);

        return _maker.make<FBOExpr>(FBOExpr::OR, lhs, rhs);
    }

    node* me::onFor(const std::string& iterName, const node& expr, const blockExpr& blk) {
        NM_DI("tokenEvent: onFor(%s, %s)", iterName, expr);

        return _maker.make<forExpr>(iterName, expr, blk);
    }

    node* me::onWhile(const node& condition, const blockExpr& blk) {
        NM_DI("tokenEvent: onWhile(%s)", condition);

        return _maker.make<whileExpr>(condition, blk);
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
        NM_DI("tokenEvent: onIn(%s, %s)", it, container);
        return _maker.make<runExpr>(container, *_maker.make<getExpr>("in"),
            args{nulOf<baseObj>(), it});
    }

    void me::onEndFunc() {
        _func.rel();
    }

    void me::onParseErr(const std::string& msg, const nchar* symbolName) {
        error(getArea().start, errCode::SYNTAX_ERR, msg.c_str(), symbolName);
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
        _dedent.setEnable(false);
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
    }

    int me::pushState(int newState) {
        NM_I("push state %d -> %d", _states.back(), newState);
        _states.push_back(newState);
        return _states.back();
    }

    int me::popState() {
        int previous = _states.back();
        _states.pop_back();
        NM_I("pop state %d <- %d", _states.back(), previous);
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
            // yyset_debug(1, scanner); // For Flex (no longer a global, but rather a member of
            // yyguts_t) yydebug = 1;             // For Bison (still global, even in a reentrant
            // parser)
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

    void me::_report(baseErr* new1) { safeGet(getReport(), add(new1)); }

    exprMaker& me::_getMaker() { return _maker; }
} // namespace nm
