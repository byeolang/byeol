#pragma once

#include "memlite/common/dep.hpp"

namespace by {

    class binder;
    struct id;
    class life;
    class instance;

    class _nhidden memliteInternal {
        BY(ME(memliteInternal))

    public:
        static void setBinderItsId(binder& me, const id& newId);
        static life* getBindTag(binder& me);
        static void setId(instance& me, const id& newId);
        static id getId(instance& me);
        static void rel(binder& me);
        static void* new1(size_t sz);
        static void del(void* pt, size_t sz);
    };
}
