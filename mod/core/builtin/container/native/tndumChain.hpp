#pragma once

#include "tnchain.inl"

namespace nm {

    template <typename K, typename V, typename defaultContainer = nmap>
    class tndumChain : public tnchain<K, V, defaultContainer> {
        typedef tnchain<K, V, defaultContainer> _super_;
        NAMU(CLASS(tndumChain, _super_))

    public:
        tndumChain() {}
        tndumChain(const super& rhs): super(rhs) {}

    public:
        // add:
        using super::add;
        nbool add(const K& key, const V& val) override { return false; }

        // link:
        using super::link;
        nbool link(const super& new1) override { return false; }

    public:
        static me singletone;
    };

    typedef tndumChain<std::string, node> ndumChain;
}
