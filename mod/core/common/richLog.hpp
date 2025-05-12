#pragma once

#include "core/common/dep.hpp"
#include "core/common/_nout.hpp"

namespace nm {
    class arithmeticObj;
    struct visitInfo;
    class node;
    typedef tstr<node> str;
    class src;
    class modifier;
    class baseFunc;
    class param;
    class params;

    _nout strWrap __convert__(const node& it);
    _nout strWrap __convert__(const node* it);
    _nout strWrap __convert__(const str& it);
    _nout strWrap __convert__(const arithmeticObj& it);
    _nout strWrap __convert__(const arithmeticObj* it);
    _nout strWrap __convert__(const src& it);
    _nout strWrap __convert__(const src* it);
    _nout strWrap __convert__(const baseFunc& it);
    _nout strWrap __convert__(const baseFunc* it);
    _nout strWrap __convert__(const param& it);
    _nout strWrap __convert__(const param* it);
    _nout strWrap __convert__(const params& it);
    _nout strWrap __convert__(const params* it);
    _nout strWrap __convert__(const visitInfo& it);
    _nout strWrap __convert__(const visitInfo* it);
    _nout strWrap __convert__(const modifier& it);
    _nout strWrap __convert__(const modifier* it);
}
