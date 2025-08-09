#pragma once

#include "core/builtin/container/native/tnchain.inl"

namespace by {

    template <typename K, typename V, typename defaultContainer = nmap>
    class tndumChain: public tnchain<K, V, defaultContainer> {
        typedef tnchain<K, V, defaultContainer> _super_;
        BY(CLASS(tndumChain, _super_))

    public:
        tndumChain() {}

        tndumChain(const super& rhs): super(rhs) {}

    public:
        using super::add;

        nbool add(const K& key, const V& val) override { return false; }

        using super::link;

        nbool link(const typename super::iter& new1) override { return false; }

    public:
        static me singleton;
    };

    typedef tndumChain<std::string, node> ndumChain;
}
