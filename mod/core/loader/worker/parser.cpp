#include "parser.hpp"
#include "bison/lowparser.hpp"
#include "bison/lowscanner.hpp"
#include "../../ast.hpp"
#include "../../ast/func.hpp"
#include "../../builtin/primitive.hpp"
#include "../../frame/thread.hpp"
#include "../../ast/genericObj.hpp"
#include "../../type/mgdType.hpp"
#include "bison/tokenScan.hpp"
#include "worker.inl"

namespace namu {

    using std::string;
    template class _nout worker<tstr<obj>, slot>;

    NAMU_DEF_ME(parser)

    namespace {
        string join(const std::vector<string>& dotnames) {
            string ret;
            for (const string& name : dotnames)
                ret += name;
            return ret;
        }
    }

    nint me::_onScan(YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner) {
        int tok = _mode->onScan(*this, val, loc, scanner);
        if (_isIgnoreWhitespace && tok == NEWLINE) return SCAN_AGAIN;
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
            //  because of definately, clang++ bug. when I use continue at switch statement inside of
            //  do-while loop here, it doesn't work like usual 'continue' keyword does, but it does like
            //  'break'.
            tok = _onScan(val, loc, scanner);
        while(tok == SCAN_AGAIN);

        return tok;
    }

    nint me::onTokenEndOfFile() {
        NAMU_DI("tokenEvent: onTokenEndOfFile() indents.size()=%d", _indents.size());
        if(_indents.size() <= 1)
            _dispatcher.add(SCAN_MODE_END);
        else
            _dispatcher.addFront(onDedent(_indents.front(), SCAN_MODE_END));

        NAMU_DI("tokenEvent: onEndOfFile: finalize by adding 'NEWLINE', then dispatch end-of-file.");
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

        NAMU_DI("tokenEvent: onTokenEndOfInlineBlock: '%c' [%d] use smart dedent!", (char) tok, tok);
        _dispatcher.addFront(tok);
        _dedent.setEnable(false);
        return NEWLINE;
    }

    nint me::onIndent(ncnt col, nint tok) {
        NAMU_DI("tokenEvent: onIndent(col: %d, tok: %d) indents.size()=%d", col, tok, _indents.size());
        _indents.push_back(col);
        _dispatcher.add(tok);
        return INDENT;
    }

    nint me::onDedent(ncnt col, nint tok) {
        NAMU_DI("tokenEvent: onDedent(col: %d, tok: %d) indents.size()=%d", col, tok, _indents.size());

        _indents.pop_back();
        nint now = _indents.back();
        if(now < col)
            posWarn(errCode::WRONG_INDENT_LV, col, now, now);

        while(_indents.back() > col) {
            NAMU_DI("tokenEvent: onDedent: indentlv become %d -> %d",
                    _indents.back(), _indents.size() > 1 ? _indents[_indents.size()-2] : -1);
            _dispatcher.add(DEDENT);
            _indents.pop_back();
            if(_indents.size() <= 0) break;
        }

        _dispatcher.add(tok);
        return DEDENT;
    }

    nint me::onTokenNewLine(nint tok) {
        NAMU_DI("tokenEvent: onNewLine: _isIgnoreWhitespace=%s, _indents.size()=%d",
            _isIgnoreWhitespace ? "true" : "false", _indents.size());
        if(!_isIgnoreWhitespace && _indents.size() >= 1)
            _dispatcher.add(SCAN_MODE_INDENT);
        _dedent.setEnable(false);
        return tok;
    }

    nchar me::onScanUnexpected(const nchar* token) {
        posError(errCode::UNEXPECTED_TOK, token);
        return token[0];
    }

    nint me::onIgnoreIndent(nint tok) {
        NAMU_DI("tokenEvent: onIgnoreIndent(%d)", tok);
        _dispatcher.add(SCAN_MODE_INDENT_IGNORE);
        return tok;
    }

    void me::_onEndWork() {
#if NAMU_IS_DBG
        const point& pt = getArea().start;
        NAMU_DI("tokenEvent: _onEndWork(%d,%d)", pt.row, pt.col);
#endif
        super::_onEndWork();
    }

    obj* me::onPack(const node& path) {
        std::vector<string> dotnames = _toDotnames(path);
        NAMU_DI("tokenEvent: onPack(%s)", join(dotnames).c_str());

        // pack syntax rule #1:
        //  if there is no specified name of pack, I create an one.
        const std::string& firstName = dotnames[0];
        if(nul(getTask()))
            _setTask(new slot(manifest(firstName)));
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
        for(int n=1; n < dotnames.size(); n++) {
            const std::string& name = dotnames[n];
            obj* sub = &e->sub<obj>(name);
            if(nul(sub))
                e->subs().add(name, sub = new obj(new mgdType(name)));
            e = sub;
        }

        return onSubPack(*e);
    }

    obj* me::onSubPack(obj& subpack) {
        NAMU_DI("tokenEvent: onSubPack()");
        _subpack.bind(subpack);
        return &subpack;
    }

    obj* me::onPack() {
        NAMU_DI("tokenEvent: onPack()");

        if(nul(getTask()))
            _setTask(new slot(manifest()));

        auto& newSlot = getTask();
        const std::string& name = getTask().getManifest().name;
        if(name != manifest::DEFAULT_NAME)
            return error(errCode::PACK_NOT_MATCH, manifest::DEFAULT_NAME, name.c_str()), &newSlot.getPack();

        return onSubPack(newSlot.getPack()); // this is a default pack containing name as '{default}'.
    }

    blockExpr* me::onBlock(const node& stmt) {
        NAMU_DI("tokenEvent: onBlock(%s)", stmt.getType().getName().c_str());
        return _maker.make<blockExpr>(stmt);
    }

    blockExpr* me::onBlock(blockExpr& blk, const node& stmt) {
        NAMU_DI("tokenEvent: onBlock(blk, %s)", stmt.getType().getName().c_str());
        if(nul(blk))
            return posError(errCode::IS_NULL, "blk"), _maker.make<blockExpr>();

        blk.getStmts().add(stmt);
        NAMU_DI("tokenEvent: onBlock(%d).add(%s)", blk.getStmts().len(), stmt.getType().getName().c_str());
        return &blk;
    }

    blockExpr* me::onBlock() {
        NAMU_DI("tokenEvent: onBlock()");
        return _maker.make<blockExpr>();
    }

    defBlock* me::onDefBlock() {
        NAMU_DI("tokenEvent: onDefBlock()");
        return new defBlock();
    }

    defBlock* me::onDefBlock(node& stmt) {
        NAMU_DI("tokenEvent: onDefBlock(%s)", stmt.getType().getName().c_str());
        return onDefBlock(*new defBlock(), stmt);
    }

    defBlock* me::onDefBlock(defBlock& s, node& stmt) {
        NAMU_DI("tokenEvent: onDefBlock(s, %s)", stmt.getType().getName().c_str());
        if(nul(s))
            return posError(errCode::IS_NULL, "s"), new defBlock();

        defPropExpr& defProp = stmt.cast<defPropExpr>();
        if(!nul(defProp)) {
            mockNode* clone = _maker.make<mockNode>(defProp.getOrigin());
            s.asScope->add(defProp.getName(), *clone);
            return &s;
        }
        defAssignExpr& defAssign = stmt.cast<defAssignExpr>();
        if(!nul(defAssign)) {
            defAssign.setTo(*_maker.make<getExpr>("me"));
            defAssign.setOnDefBlock(true);
            s.asPreCtor->add(defAssign);
            return &s;
        }

        s.asScope->add(stmt.getSrc().getName(), &stmt);
        return &s;
    }

    node* me::onDefVar(const std::string& name, const node& origin) {
        NAMU_DI("tokenEvent: onDefVar(%s, %s)", origin.getType().getName().c_str(), name.c_str());

        return _maker.make<defPropExpr>(name, origin);
    }

    node* me::onDefArray(const narr& items) {
        NAMU_DI("tokenEvent: onDefArray(items.len[%d])", items.len());
        return _maker.make<defArrayExpr>(items);
    }

    node* me::onDefSeq(const node& start, const node& end) {
        NAMU_DI("tokenEvent: onDefSeq()");

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
        for(auto& a : as) {
            tstr<defPropExpr> defProp(a.cast<defPropExpr>());
            if(!defProp)
                return posError(errCode::PARAM_HAS_VAL), ret;

            ret.add(new param(defProp->getName(), defProp->getOrigin()));
        }

        return ret;
    }

    func* me::onAbstractFunc(const getExpr& access, const node& retType) {
        NAMU_DI("tokenEvent: onAbstractFunc(access: %s(%d), retType:%s)",
                access.getSubName().c_str(), access.getSubArgs().len(), retType.getType().getName().c_str());

        return _maker.birth<func>(access.getSubName(), _asParams(access.getSubArgs()), retType);
    }

    func* me::onAbstractFunc(node& it, const node& retType) {
        NAMU_DI("tokenEvent: onAbstractFunc(it: %s, retType: %s)", it.getType().getName().c_str(), retType.getType().getName().c_str());
        return onAbstractFunc(onCallAccess(it, *new narr())->cast<getExpr>(), retType);
    }

    func* me::onFunc(func& f, const blockExpr& blk) {
        NAMU_DI("tokenEvent: onFunc: func[%x] blk.len()=%d", (void*) &f, blk.getStmts().len());

        f.setBlock(blk);
        return &f;
    }

    narr* me::onParams() {
        NAMU_DI("tokenEvent: onParams()");
        return new narr();
    }
    narr* me::onParams(const defPropExpr& elem) {
        NAMU_DI("tokenEvent: onParams(%s %s)", elem.getName().c_str(), elem.getOrigin().getType().getName().c_str());
        narr* ret = new narr();
        ret->add(elem);
        return ret;
    }
    narr* me::onParams(narr& it, const defPropExpr& elem) {
        NAMU_DI("tokenEvent: onParams(narr(%d), %s %s)", it.len(), elem.getName().c_str(), elem.getOrigin().getType().getName().c_str());
        it.add(elem);
        return &it;
    }

    node* me::onParanthesisAsTuple(narr& tuple) {
        NAMU_DI("tokenEnvent: onParanthesisAsTuple(%x.size=%d)", &tuple, tuple.len());

        if(tuple.len() != 1) {
            error(errCode::PARANTHESIS_WAS_TUPLE);
            return new mockNode();
        }

        return &tuple[0];
    }

    args* me::onTuple() {
        NAMU_DI("tokenEvent: onTuple()");
        return new args();
    }

    args* me::onTuple(const node& elem) {
        NAMU_DI("tokenEvent: onTuple(%s[%x])", elem.getType().getName().c_str(), &elem);
        args* ret = new args();
        ret->add(elem);
        return ret;
    }

    args* me::onTuple(args& as, const node& elem) {
        NAMU_DI("tokenEvent: onTuple(as[%x], %s[%x])", &as, elem.getType().getName().c_str(), &elem);
        as.add(elem);
        return &as;
    }

    args* me::onFuncCallTuple() {
        NAMU_DI("tokenEvent: onFuncCallTuple()");
        return new args();
    }

    args* me::onFuncCallTuple(const node& elem) {
        NAMU_DI("tokenEvent: onFuncCallTuple(elem[%s]=%x)", elem.getType().getName().c_str(), &elem);
        args* ret = new args();
        ret->add(elem);
        return ret;
    }

    args* me::onFuncCallTuple(args& as, const node& elem) {
        NAMU_DI("tokenEvent: onFuncCallTuple(as[%x], elem[%s])", &as, elem.getType().getName().c_str());
        as.add(elem);
        return &as;
    }

    args* me::onTypeNames(const node& param) {
        NAMU_DI("tokenEvent: onTypeNames(%s)", param.getType().getName().c_str());
        auto* ret = new args();
        ret->add(param);
        return ret;
    }

    args* me::onTypeNames(args& params, const node& param) {
        NAMU_DI("tokenEvent: onTypeNames(len[%d], %s)", params.len(), param.getType().getName().c_str());
        params.add(param);
        return &params;
    }

    obj* me::onDefObj(const std::string& name, defBlock& blk) {
        NAMU_DI("tokenEvent: onDefObj(%s, defBlock[%x])", name.c_str(), &blk);

        obj& ret = *_maker.birth<obj>(name, new mgdType(name));
        ret._setComplete(false);
        ret._setSubPack(*_subpack);
        _onInjectObjSubs(ret, blk);
        return &ret;
    }

    std::vector<std::string> me::_extractParamTypeNames(const args& types) {
        std::vector<std::string> ret;
        for(const auto& a : types) {
            // all args should be getExpr instances.
            const getExpr& cast = a.cast<getExpr>();
            if(nul(cast))
                return posError(errCode::SHOULD_TYPE_PARAM_NAME, a.getType().getName().c_str()),
                       std::vector<std::string>();

            ret.push_back(cast.getSubName());
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
            ret.push_back(iter->getSubName());
            const node& next = iter->getMe();
            if(!nul(next) && !next.is<getExpr>()) {
                error(errCode::PACK_ONLY_ALLOW_VAR_ACCESS);
                return std::vector<string>();
            }
            iter = &next.cast<getExpr>();
        } while(iter);
        return ret;
    }

    node* me::onDefObjGeneric(const std::string& name, const args& typeParams, defBlock& blk) {
        NAMU_DI("tokenEvent: onDefObjGeneric(%s, type.len[%d], defBlock[%x]", name.c_str(),
                typeParams.len(), &blk);

        obj& org = *_maker.birth<obj>(name, new mgdType(name, typeParams));
        org._setComplete(false);
        _onInjectObjSubs(org, blk);
        org._setSubPack(*_subpack);

        std::vector<std::string> paramNames = _extractParamTypeNames(typeParams);
        std::string paramName = _joinVectorString(paramNames);
        return _maker.birth<genericObj>(name, org, paramNames);
    }

    std::string me::_joinVectorString(const std::vector<std::string>& container) const {
        std::string ret;
        for(ncnt n=0; n < container.size(); n++) {
            ret += container[n];
            if(n > 0)
                ret += ", ";
        }
        return ret;
    }

    void me::onCompilationUnit(obj& subpack) {
        NAMU_DI("tokenEvent: onCompilationUnit(%x)", &subpack);

        tstr<defBlock> blkLife(new defBlock());
        _onCompilationUnit(subpack, *blkLife);
    }

    void me::onCompilationUnit(obj& subpack, defBlock& blk) {
        NAMU_DI("tokenEvent: onCompilationUnit(%x, defBlock[%x].preCtor.len()=%d)",
                &subpack, &blk, blk.asPreCtor ? blk.asPreCtor->len() : 0);

        _onCompilationUnit(subpack, blk);
    }

    void me::_onCompilationUnit(obj& subpack, defBlock& blk) {
        if(nul(subpack))
            return error(errCode::NO_PACK_TRAY), void();

        _onInjectObjSubs(subpack, blk);

        // link system slots:
        subpack.getShares().link(thread::get().getSlots());
        NAMU_DI("link system slots[%d]: len=%d", thread::get().getSlots().len(), subpack.subs().len());

        // at this far, subpack must have at least 1 default ctor created just before:
        NAMU_DI("tokenEvent: onCompilationUnit: run preconstructor(%d lines)",
                (!nul(blk) && blk.asPreCtor) ? blk.asPreCtor->len() : 0);
        subpack.run(baseObj::CTOR_NAME); // don't need argument. it's default ctor.
    }

    nbool me::_onInjectObjSubs(obj& it, defBlock& blk) {
        NAMU_DI("tokenEvent: _onInjectObjSubs(%s, defBlock[%x])", it.getType().getName().c_str(), &blk);
        if(nul(it)) return false;

        bicontainable& share = it.getShares().getContainer();
        bicontainable& own = it.getOwns();
        for(auto e=blk.asScope->begin(); e ;++e) {
            // TODO: not only func, but also shared variable.
            bicontainable& con = nul(e.getVal<baseFunc>()) ? own : share;
            con.add(e.getKey(), *e);
        }

        return _onInjectCtor(it, blk);
    }

    nbool me::_onInjectCtor(obj& it, defBlock& blk) {
        nbool hasCtor = !nul(it.sub(baseObj::CTOR_NAME));
        NAMU_DI("tokenEvent: _onInjectDefaultCtor(%s, has=%d)", it.getType().getName().c_str(), hasCtor);
        if(hasCtor) return false;

        // TODO: ctor need to call superclass's ctor.
        it.getShares().getContainer().add(baseObj::CTOR_NAME, *_maker.make<defaultCtor>(it.getOrigin()));
        it.getShares().getContainer().add(baseObj::CTOR_NAME, *_maker.make<defaultCopyCtor>(it.getOrigin()));

        // add preCtor:
        if(blk.asPreCtor && blk.asPreCtor->len()) {
            func* preCtor = _maker.make<func>(params(), new nVoid());
            preCtor->getBlock().getStmts().add(*blk.asPreCtor);
            it.subs().add(baseObj::PRECTOR_NAME, preCtor);
        }
        return true;
    }

    retExpr* me::onRet() {
        NAMU_DI("tokenEvent: onRet()");

        return _maker.make<retExpr>();
    }

    retExpr* me::onRet(node& exp) {
        NAMU_DI("tokenEvent: onRet(%s)", exp.getType().getName().c_str());

        return _maker.make<retExpr>(exp);
    }

    breakExpr* me::onBreak() {
        NAMU_DI("tokenEvent: onBreak()");

        return _maker.make<breakExpr>();
    }

    nextExpr* me::onNext() {
        NAMU_DI("tokenEvent: onNext()");

        return _maker.make<nextExpr>();
    }

    node* me::onGet(const std::string& name) {
        NAMU_DI("tokenEvent: onGet(%s)", name.c_str());
        return _maker.make<getExpr>(name);
    }

    node* me::onGet(const std::string& name, const narr& args) {
        NAMU_DI("tokenEvent: onGet(%s, %d)", name.c_str(), args.len());
        return _maker.make<getExpr>(name, args);
    }

    node* me::onGet(node& from, node& it) {
        getExpr& cast = it.cast<getExpr>();
        if(nul(cast)) {
            error(errCode::IDENTIFIER_ONLY, it.getType().getName().c_str());
            return &from;
        }

        NAMU_DI("tokenEvent: onGet(%s, %s)", from.getType().getName().c_str(), cast.getSubName().c_str());
        cast.setMe(from);
        return &cast;
    }

    node* me::onCallAccess(node& it, const narr& as) {
        getExpr& cast = it.cast<getExpr>();
        if(nul(cast)) {
            // it can be generic or primitive values. track it, leave as specific errs.
            error(errCode::IDENTIFIER_ONLY, it.getType().getName().c_str());
            return new getExpr("");
        }

        cast.setSubArgs(*new args(as));
        return &cast;
    }

    node* me::onGetArray(node& elemType) {
        NAMU_DI("tokenEvent: onGetArray(%s)", elemType.getType().getName().c_str());
        if(elemType.isSub<nVoid>()) error(elemType.getSrc().getPos(), errCode::ELEM_TYPE_NOT_VOID);
        return new arr(elemType);
    }

    node* me::onGetElem(const node& arr, const node& idx) {
        NAMU_DI("tokenEvent: onGetElem(%s, %s)", arr.getType().getName().c_str(),
                idx.getType().getName().c_str());

        return _maker.make<runExpr>(arr, *_maker.make<getExpr>(arr, "get", *new args{narr{idx}}),
                args{narr{idx}});
    }

    node* me::onGetGeneric(const std::string& genericObjName, const args& typeParams) {
        NAMU_DI("tokenEvent: onGetGeneric(%s, params.len[%d])", genericObjName.c_str(), typeParams.len());
        return _maker.make<getGenericExpr>(genericObjName, typeParams);
    }

    runExpr* me::onRunExpr(node& trg, const narr& a) {
        NAMU_DI("tokenEvent: onRunExpr(%s, narr[%d])", trg.getType().getName().c_str(), a.len());
        return _onRunExpr(trg, *new args(a));
    }
    runExpr* me::onRunExpr(node& trg, const args& a) {
        NAMU_DI("tokenEvent: onRunExpr(%s, args[%d])", trg.getType().getName().c_str(), a.len());
        return _onRunExpr(trg, a);
    }

    // @param from  can be expr. so I need to evaluate it through 'as()'.
    runExpr* me::onFillFromOfFuncCall(const node& me, runExpr& to) {
        NAMU_DI("tokenEvent: onFillFromOfFuncCall(%s, runExpr[%x])", me.getType().getName().c_str(), &to);

        to.setMe(me);
        return &to;
    }

    node* me::onAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        // _onSetElem branch:
        //  if user code is 'arr[0] = 1', then it will be interpreted to 'arr.set(0, 1)'
        runExpr& cast = lhs.cast<runExpr>();
        if(!nul(cast)) {
            getExpr& subject = cast.getSubject().cast<getExpr>();
            if(!nul(subject))
                if(subject.getSubName() == "get")
                    return _onSetElem(cast, rhs);
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
        NAMU_DI("tokenEvent:: _onSetElem(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());

        getExpr& subject = lhs.getSubject().cast<getExpr>();
        subject._name = "set";
        subject._args.rel();
        lhs.getArgs().add(rhs);

        return &lhs;
    }

    node* me::_onAssignElem(FBOExpr::rule type, node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onFBOAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());

        // _onConvertAssignElem branch:
        //  if user code is 'arr[0] = 1', then it will be interpreted to 'arr.set(0, 1)'
        runExpr& cast = lhs.cast<runExpr>();
        if(!nul(cast)) {
            getExpr& subject = cast.getSubject().cast<getExpr>();
            if(!nul(subject))
                if(subject.getSubName() == "get")
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
        NAMU_DI("tokenEvent: _onConvertAssignElem(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());

        runExpr& setter = *(runExpr*) lhs.clone();
        getExpr& newSubj = *(getExpr*) setter.getSubject().clone();
        newSubj._name = "set";
        newSubj._args.rel();
        setter._subject.bind(newSubj);
        setter.getArgs().add(rhs);
        return &setter;
    }

    runExpr* me::_onRunExpr(node& trg, const args& a) {
        runExpr* ret = _maker.make<runExpr>(nulOf<node>(), trg, a);
        getExpr& cast = trg.cast<getExpr>();
        if(!nul(cast) && !cast.isSub<getGenericExpr>())
            cast.setSubArgs(a);
        return ret;
    }

    node* me::onAddAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onAddAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        return _onAssignElem(FBOExpr::ADD, lhs, rhs);
    }

    node* me::onSubAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onSubAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        return _onAssignElem(FBOExpr::SUB, lhs, rhs);
    }

    node* me::onMulAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onMulAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        return _onAssignElem(FBOExpr::MUL, lhs, rhs);
    }

    node* me::onDivAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onDivAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        return _onAssignElem(FBOExpr::DIV, lhs, rhs);
    }

    node* me::onModAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onModAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        return _onAssignElem(FBOExpr::MOD, lhs, rhs);
    }

    node* me::onDefAssign(const std::string& name, node& rhs) {
        NAMU_DI("tokenEvent: onDefAssign(%s, %s)", name.c_str(), rhs.getType().getName().c_str());

        return _maker.make<defAssignExpr>(name, rhs);
    }

    asExpr* me::onAs(const node& me, const node& as) {
        NAMU_DI("tokenEvent: onAs(%s, %s)", me.getType().getName().c_str(), as.getType().getName().c_str());

        return _maker.make<asExpr>(me, as);
    }

    FBOExpr* me::onUnaryMinus(const node& it) {
        NAMU_DI("tokenEvent: onUnaryMinus(%s)", it.getType().getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::MUL, it, *new nInt(-1));
    }

    node* me::onUnaryDoublePlus(node& it) {
        NAMU_DI("tokenEvent: onUnaryDoublePlus(%s)", it.getType().getName().c_str());

        return onAssign(it, *_maker.make<FBOExpr>(FBOExpr::ADD, it, *new nInt(1)));
    }

    node* me::onUnaryDoubleMinus(node& it) {
        NAMU_DI("tokenEvent: onUnaryDoubleMinus(%s)", it.getType().getName().c_str());

        return onAssign(it, *_maker.make<FBOExpr>(FBOExpr::SUB, it, *new nInt(1)));
    }

    FUOExpr* me::onUnaryPostfixDoublePlus(const node& it) {
        NAMU_DI("tokenEvent: onUnaryPostfixDoublePlus(%s)", it.getType().getName().c_str());

        return _maker.make<FUOExpr>(FUOExpr::POSTFIX_DOUBLE_PLUS, it);
    }

    FUOExpr* me::onUnaryPostfixDoubleMinus(const node& it) {
        NAMU_DI("tokenEvent: onUnaryPostfixDoubleMinus(%s)", it.getType().getName().c_str());

        return _maker.make<FUOExpr>(FUOExpr::POSTFIX_DOUBLE_MINUS, it);
    }

    FBOExpr* me::onUnaryNot(const node& it) {
        NAMU_DI("tokenEvent: onUnaryNot(%s)", it.getType().getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::EQ, it, *new nBool(false));
    }

    FUOExpr* me::onUnaryBitwiseNot(const node& it) {
        NAMU_DI("tokenEvent: onUnaryBitwiseNot(%s)", it.getType().getName().c_str());

        return _maker.make<FUOExpr>(FUOExpr::BITWISE_NOT, it);
    }

    FBOExpr* me::onAdd(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onAdd(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::ADD, lhs, rhs);
    }

    FBOExpr* me::onSub(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onSub(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::SUB, lhs, rhs);
    }

    FBOExpr* me::onMul(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onMul(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::MUL, lhs, rhs);
    }

    FBOExpr* me::onDiv(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onDiv(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::DIV, lhs, rhs);
    }

    FBOExpr* me::onMod(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onMod(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::MOD, lhs, rhs);
    }

    FBOExpr* me::onBitwiseAnd(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onBitwiseAnd(%s, %s)", lhs.getType().getName().c_str(), rhs.getType()
                .getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::BITWISE_AND, lhs, rhs);
    }

    FBOExpr* me::onBitwiseOr(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onBitwiseOr(%s, %s)", lhs.getType().getName().c_str(), rhs.getType()
                .getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::BITWISE_OR, lhs, rhs);
    }

    FBOExpr* me::onBitwiseXor(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onBitwiseXor(%s, %s)", lhs.getType().getName().c_str(), rhs.getType()
                .getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::BITWISE_XOR, lhs, rhs);
    }

    FBOExpr* me::onLShift(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: LShift(%s, %s)", lhs.getType().getName().c_str(), rhs.getType()
                .getName().c_str());

       return _maker.make<FBOExpr>(FBOExpr::LSHIFT, lhs, rhs);
    }

    FBOExpr* me::onRShift(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: RShift(%s, %s)", lhs.getType().getName().c_str(), rhs.getType()
                .getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::RSHIFT, lhs, rhs);
    }

    FBOExpr* me::onGt(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onGt(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::GT, lhs, rhs);
    }

    FBOExpr* me::onLt(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onLt(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::LT, lhs, rhs);
    }

    FBOExpr* me::onGe(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onGe(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::GE, lhs, rhs);
    }

    FBOExpr* me::onLe(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onLe(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::LE, lhs, rhs);
    }

    FBOExpr* me::onEq(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onEq(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::EQ, lhs, rhs);
    }

    FBOExpr* me::onNe(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onNe(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::NE, lhs, rhs);
    }

    FBOExpr* me::onAnd(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onAnd(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::AND, lhs, rhs);
    }

    FBOExpr* me::onOr(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onOr(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::OR, lhs, rhs);
    }

    node* me::onFor(const std::string& iterName, const node& expr, const blockExpr& blk) {
        NAMU_DI("tokenEvent: onFor(%s, %s)", iterName.c_str(), expr.getType().getName().c_str());

        return _maker.make<forExpr>(iterName, expr, blk);
    }

    node* me::onWhile(const node& condition, const blockExpr& blk) {
        NAMU_DI("tokenEvent: onWhile(%s)", condition.getType().getName().c_str());

        return _maker.make<whileExpr>(condition, blk);
    }

    ifExpr* me::onIf(const node& condition, const blockExpr& then) {
        NAMU_DI("tokenEvent: onIf(then)");

        return _maker.make<ifExpr>(condition, then);
    }
    ifExpr* me::onIf(const node& condition, const blockExpr& then, const blockExpr& elseBlk) {
        NAMU_DI("tokenEvent: onIf(condition, then, elseBlk)");
        ifExpr* ret = onIf(condition, then);

        ret->setElseBlk(elseBlk);
        return ret;
    }
    ifExpr* me::onIf(const node& condition, const blockExpr& then, const ifExpr& elseIf) {
        NAMU_DI("tokenEvent: onIf(condition, then, elseIf)");
        ifExpr* ret = onIf(condition, then);
        ret->setElseBlk(*new blockExpr(elseIf));
        return ret;
    }

    runExpr* me::onIn(const node& it, const node& container) {
        NAMU_DI("tokenEvent: onIn(%s, %s)", it.getType().getName().c_str(), container.getType().getName().c_str());
        return _maker.make<runExpr>(container, *_maker.make<getExpr>("has"), args(narr{it}));
    }

    void me::onParseErr(const std::string& msg, const nchar* symbolName) {
        error(getArea().start, errCode::SYNTAX_ERR, msg.c_str(), symbolName);
    }

    me::parser() { rel(); }

    obj& me::getSubPack() {
        return *_subpack; // TODO: can I remove subpack variable?
    }

    srcSupplies& me::getSrcSupplies() {
        return _supplies;
    }

    me& me::addSupply(const srcSupply& new1) {
        _supplies.add(new1);
        return *this;
    }

    me& me::relSupplies() {
        _supplies.rel();
        return *this;
    }

    tokenDispatcher& me::getDispatcher() {
        return _dispatcher;
    }

    std::vector<ncnt>& me::getIndents() {
        return _indents;
    }
    nbool me::isInit() const {
        return _mode;
    }

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
        NAMU_I("push state %d -> %d", _states.back(), newState);
        _states.push_back(newState);
        return _states.back();
    }


    int me::popState() {
        int previous = _states.back();
        _states.pop_back();
        NAMU_I("pop state %d <- %d", _states.back(), previous);
        return _states.back();
    }

    tstr<obj> me::_onWork() {
        const auto& supplies = getSrcSupplies();
        if(supplies.isEmpty())
            return error(NO_SRC), tstr<obj>();

        NAMU_I("parse starts: %d src will be supplied.", supplies.len());
        for(const auto& supply : supplies) {
            _prepare();

            yyscan_t scanner;
            yylex_init_extra(this, &scanner);

            YY_BUFFER_STATE bufState = (YY_BUFFER_STATE) supply.onSupplySrc(*this, scanner);
            if(!bufState) {
                error(errCode::IS_NULL, "bufState");
                return tstr<obj>();
            }

            // fix Flex Bug here:
            //  when yy_scan_string get called, it returns bufState after malloc it.
            //  but some variables wasn't initialized. yy_bs_lineno(used to calculate
            //  current cursor position) is one of them.
            bufState->yy_bs_lineno = bufState->yy_bs_column = 0;
            yy_switch_to_buffer(bufState, scanner);

#if YYDEBUG
            //yyset_debug(1, scanner); // For Flex (no longer a global, but rather a member of yyguts_t)
            //yydebug = 1;             // For Bison (still global, even in a reentrant parser)
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

    void me::_report(err* new1) {
        errReport& rpt = getReport();
        if(nul(rpt)) return;

        rpt.add(new1);
    }

    exprMaker& me::_getMaker() {
        return _maker;
    }
}
