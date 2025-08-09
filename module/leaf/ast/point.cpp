#include "leaf/ast/point.hpp"

namespace by {

    BY(DEF_ME(point))

    nbool me::isOrigin() const { return row == 0 && col == 0; }

    void me::rel() { row = col = 0; }

    me& me::operator++() {
        ++row;
        ++col;
        return *this;
    }

    void area::rel() {
        start.rel();
        end.rel();
    }

    area& area::operator++() {
        ++start;
        ++end;
        return *this;
    }
}
