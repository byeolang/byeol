#include "line.hpp"
#include "indep/macro/when.hpp"

namespace by {

    thread_local ncnt _lv = 0;

    static constexpr ncnt _MAX_LV = 20;
    static std::string _LINES[_MAX_LV + 1] = {
        "",                     // 0
        "┣ ",                   // 1
        "┃┣ ",                  // 2
        "┃┃┣ ",                 // 3
        "┃┃┃┣ ",                // 4
        "┃┃┃┃┣ ",               // 5
        "┃┃┃┃┃┣ ",              // 6
        "┃┃┃┃┃┃┣ ",             // 7
        "┃┃┃┃┃┃┃┣ ",            // 8
        "┃┃┃┃┃┃┃┃┣ ",           // 9
        "┃┃┃┃┃┃┃┃┃┣ ",          // 10
        "┃┃┃┃┃┃┃┃┃┃┣ ",         // 11
        "┃┃┃┃┃┃┃┃┃┃┃┣ ",        // 12
        "┃┃┃┃┃┃┃┃┃┃┃┃┣ ",       // 13
        "┃┃┃┃┃┃┃┃┃┃┃┃┃┣ ",      // 14
        "┃┃┃┃┃┃┃┃┃┃┃┃┃┃┣ ",     // 15
        "┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┣ ",    // 16
        "┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┣ ",   // 17
        "┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┣ ",  // 18
        "┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┣ ", // 19
        "┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┃┣ " // 20
    };

    void line::incLv() {
        WHEN(_lv >= _MAX_LV) .ret();
        _lv++;
    }

    void line::decLv() {
        WHEN(_lv <= 0) .ret();
        _lv--;
    }

    ncnt line::getLv() { return _lv; }

    const std::string& line::getLine() {
        ncnt lv = _lv <= _MAX_LV ? _lv : _MAX_LV;
        return _LINES[lv];
    }

} // namespace by
