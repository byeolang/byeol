#include "leaf/parser/leafParser.hpp"
#include "leaf/ast.hpp"
#include "leaf/parser/bison/leafLowparser.hpp"
#include "leaf/parser/bison/leafLowscanner.hpp"
#include "leaf/parser/bison/leafTokenScan.hpp"
#include <algorithm>
#include <cctype>

namespace nm {

    using std::string;

    NM_DEF_ME(leafParser)

    nint me::_onScan(ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner) {
        int tok = _mode->onScan(*this, val, loc, scanner);
        if(_isIgnoreWhitespace && tok == NEWLINE) return SCAN_AGAIN;
        _isIgnoreWhitespace = false;

        switch(tok) {
            case SCAN_MODE_NORMAL: setScan<leafNormalScan>(); return SCAN_AGAIN;
            case SCAN_MODE_INDENT: setScan<leafIndentScan>(); return SCAN_AGAIN;
            case SCAN_MODE_INDENT_IGNORE: _isIgnoreWhitespace = true; return SCAN_AGAIN;
            case SCAN_MODE_END: tok = 0; // == zzterminate();
        }

        return tok;
    }

    nint me::onScan(leafParser& ev, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner, nbool& isBypass) {
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

    nint me::onTokenNewLine(nint tok) {
        NM_DI("tokenEvent: onNewLine: _isIgnoreWhitespace=%s, _indents.size()=%d",
            _isIgnoreWhitespace, _indents.size());
        if(!_isIgnoreWhitespace && _indents.size() >= 1) _dispatcher.add(SCAN_MODE_INDENT);
        _dedent.rel();
        return tok;
    }

    nint me::onTokenComma(nint tok) { return _onTokenEndOfInlineBlock(onIgnoreIndent(tok)); }

    leaf* me::onDefAssign(const std::string& name, leaf* rhs) {
        WHEN_NUL(rhs).err("rhs is nul").ret(nullptr);
        rhs->setName(name);
        return rhs;
    }

    nint me::_onTokenEndOfInlineBlock(nint tok) {
        WHEN(!_dedent.canDedent()).ret(tok);

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
        if(now < col) report("wrong dedent lv");

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

    nchar me::onScanUnexpected(const nchar* token) {
        report("unexpected_tok");
        return token[0];
    }

    nint me::onIgnoreIndent(nint tok) {
        _dispatcher.add(SCAN_MODE_INDENT_IGNORE);
        return tok;
    }

    leaf* me::onDefBlock() { return new leaf(); }

    leaf* me::onDefBlock(leaf* stmt) { return onDefBlock(onDefBlock(), stmt); }

    leaf* me::onDefBlock(leaf* s, leaf* stmt) {
        WHEN_NUL(s).err("s is nul").ret(new leaf());
        WHEN_NUL(stmt).err("stmt is nul").ret(s);

        s->add(stmt);
        return s;
    }

    verLeaf* me::onVer(const std::string& version) { return new verLeaf(version); }

    leaf* me::onDefProp(const std::string& name, leaf& rhs) {
        rhs.setName(name);
        return &rhs;
    }

    leaf* me::onDefArray(const leaf& elem) { return new leaf{elem}; }

    leaf* me::onDefArray(leaf& as, const leaf& elem) {
        as.add(elem);
        return &as;
    }

    leaf* me::onDefOrigin(const std::string& name, leaf& blk) {
        blk.setName(name);
        return &blk;
    }

    leaf* me::onCompilationUnit(leaf* subpack) {
        WHEN_NUL(subpack).err("subpack is null").ret(nullptr);

        subpack->setName("root");
        _root.bind(subpack);
        return subpack;
    }

    void me::onParseErr(const std::string& msg, const nchar* symbolName) {
        report(msg + " -> " + symbolName);
    }

    me::leafParser(): _mode(nullptr), _isIgnoreWhitespace(false) { rel(); }

    leafTokenDispatcher& me::getDispatcher() { return _dispatcher; }

    std::vector<ncnt>& me::getIndents() { return _indents; }

    tstr<leaf> me::parseFromFile(const std::string& path) {
        std::ifstream fout(path);
        if(fout.fail()) {
            // there is no file.
            report(path + " not exist.");
            return nullptr;
        }

        std::stringstream buf;
        buf << fout.rdbuf();
        return parse(buf.str());
    }

    tstr<leaf> me::parseFromFile(const nchar* path) {
        WHEN_NUL(path).ret(nullptr);
        return parseFromFile(path);
    }

    tstr<leaf> me::parse(const std::string& codes) {
        _prepare();

        zzscan_t scanner;
        zzlex_init_extra(this, &scanner);

        yy_buffer_state& bufState =
            (YY_BUFFER_STATE) _scanString(codes.c_str(), scanner) OR.ret(nullptr);

        // fix Flex Bug here:
        //  when zz_scan_string get called, it returns bufState after malloc it.
        //  but some variables wasn't initialized. zz_bs_lineno(used to calculate
        //  current cursor position) is one of them.
        bufState.yy_bs_lineno = bufState.yy_bs_column = 0;
        zz_switch_to_buffer(&bufState, scanner);

#if ZZDEBUG
        // zzset_debug(1, scanner); // For Flex (no longer a global, but rather a member of)
        // zzdebug = 1;             // For Bison (still global, even in a reentrant)
#endif

        int res = zzparse(scanner);
        if(res) report("parsing has error");

        zz_delete_buffer(&bufState, scanner);
        zzlex_destroy(scanner);

        return _finalize();
    }

    tstr<leaf> me::parse(const nchar* codes) {
        WHEN_NUL(codes).ret(nullptr);
        return parse(codes);
    }

    nbool me::isInit() const { return _mode; }

    void me::rel() {
        _states.clear();
        _states.push_back(0); // 0 for default state
        _dedent.rel();
        _prepare();
    }

    void me::_prepare() {
        _mode = nullptr;
        _root.rel();
        _isIgnoreWhitespace = false;
        _dispatcher.rel();
        _indents.clear();
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

    void me::report(const std::string& msg) { _errs.push_back(msg); }

    void* me::_scanString(const nchar* src, void* scanner) {
        if(nul(src) || src[0] == '\0') {
            report("src is empty");
            return nullptr;
        }

        return zz_scan_string((nchar*) src, (zzscan_t) scanner);
    }

    tstr<leaf> me::_finalize() {
        ncnt size = _errs.size();
        WHEN(size <= 0).ret(_root);

        NM_I("leaf: total %d errors found.", size);
        for(const auto& e: _errs)
            NM_I("leaf: ERR: %s", e);
        return nullptr;
    }
} // namespace nm
