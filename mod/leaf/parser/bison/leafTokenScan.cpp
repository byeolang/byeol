#include "leafTokenScan.hpp"

#include <iostream>

#include "leafLowscanner.hpp"

YY_DECL;

std::string getTokenName(int tok);

namespace nm {

    nint leafTokenScan::onScan(leafParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner,
        nbool& isBypass) {
        leafTokenDispatcher& disp = ps.getDispatcher();
        nint tok;

        if(!(isBypass = disp.pop(tok))) tok = zzlexOrigin(val, loc, scanner);
        if(tok == ENDOFFILE) tok = ps.onTokenEndOfFile();

        std::string tokName = getTokenName(tok);
        NM_DI("leaf: dispatcher[%d]%s(token: \"%s\" [%c]) at %d,%d", disp.len(),
            isBypass ? ".dispatch" : " lowscanner", tokName, tok, loc->start.row, loc->start.col);
        return tok;
    }

    nint leafNormalScan::onScan(leafParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner,
        nbool& isBypass) {
        nint tok = super::onScan(ps, val, loc, scanner, isBypass);
        switch(tok) {
            case TAB: return SCAN_AGAIN;
        }

        return tok;
    }

    leafNormalScan* leafNormalScan::_instance = new leafNormalScan();

    nint leafIndentScan::onScan(leafParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner,
        nbool& isBypass) {
        nint tok = super::onScan(ps, val, loc, scanner, isBypass);
        switch(tok) {
            case NEWLINE:
                if(!isBypass) {
                    NM_DI("leafIndentScan: ignore NEWLINE");
                    return SCAN_AGAIN;
                }
                break;
        }

        ps.setScan<leafNormalScan>();
        if(isBypass) return tok;

        ncnt cur = loc->start.col;
        std::vector<ncnt>& ind = ps.getIndents();
        if(ind.size() == 0) {
            NM_DI("leafIndentScan: initial indent lv: %d", cur);
            ind.push_back(cur);
        }

        ncnt prev = ind.back();
        NM_DI("leafIndentScan: column check: cur[%d] prev[%d]", cur, prev);

        if(cur > prev) return ps.onIndent(cur, tok);
        else if(cur < prev) return ps.onDedent(cur, tok);

        return tok;
    }

    leafIndentScan* leafIndentScan::_instance = new leafIndentScan();

} // namespace nm
