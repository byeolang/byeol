#pragma once

#include "_nout.hpp"
#include "dep.hpp"

namespace nm {

    const std::string MANIFEST_FILENAME = "manifest.seedling";

    class node;
    class ntype;
    typedef tstr<node> str;
    typedef tweak<node> weak;
}
