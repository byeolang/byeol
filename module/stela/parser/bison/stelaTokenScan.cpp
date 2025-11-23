#include "stela/parser/bison/stelaTokenScan.hpp"
#include <iostream>
#include "stela/parser/bison/stelaLowscanner.hpp"

YY_DECL;

std::string getStelaTokenName(int tok);

namespace by {

    nint stelaTokenScan::onScan(stelaParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner,
        nbool& isBypass) {
        stelaTokenDispatcher& disp = ps.getDispatcher();
        nint tok;

        if(!(isBypass = disp.pop(tok))) tok = zzlexOrigin(val, loc, scanner);
        if(tok == ENDOFFILE) tok = ps.onTokenEndOfFile();

        std::string tokName = getStelaTokenName(tok);
        BY_DI("stela: dispatcher[%d]%s(token: \"%s\" [%d]) at %d,%d", disp.len(),
            isBypass ? ".dispatch" : " lowscanner", tokName, tok, loc->start.row, loc->start.col);
        return tok;
    }

    nint stelaNormalScan::onScan(stelaParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner,
        nbool& isBypass) {
        nint tok = super::onScan(ps, val, loc, scanner, isBypass);
        switch(tok) {
            case TAB: return SCAN_AGAIN;
        }

        return tok;
    }

    stelaNormalScan* stelaNormalScan::_instance = new stelaNormalScan();

    nint stelaIndentScan::onScan(stelaParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner,
        nbool& isBypass) {
        nint tok = super::onScan(ps, val, loc, scanner, isBypass);
        switch(tok) {
            case NEWLINE:
                if(!isBypass) {
                    BY_DI("stelaIndentScan: ignore NEWLINE");
                    return SCAN_AGAIN;
                }
                break;
        }

        ps.setScan<stelaNormalScan>();
        if(isBypass) return tok;

        ncnt cur = loc->start.col;
        std::vector<ncnt>& ind = ps.getIndents();
        if(ind.size() == 0) {
            BY_DI("stelaIndentScan: initial indent lv: %d", cur);
            ind.push_back(cur);
        }

        ncnt prev = ind.back();
        BY_DI("stelaIndentScan: column check: cur[%d] prev[%d]", cur, prev);

        if(cur > prev) return ps.onIndent(cur, tok);
        else if(cur < prev) return ps.onDedent(cur, tok);

        return tok;
    }

    stelaIndentScan* stelaIndentScan::_instance = new stelaIndentScan();

} // namespace by
