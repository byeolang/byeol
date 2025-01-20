#pragma once

#include <unordered_map>
#include "../../../common.hpp"

namespace nm {
    /// smultimap:
    ///     a multimap container class that preserves insertion order.
    ///     in C++, a general map operates regardless of the insertion order when iteration is
    ///     performed.
    ///
    ///     this class manages key and value as a pair like multimap, but when iteration is performed,
    ///     it remembers the insertion order and iterates like a queue. This function is used in
    ///     arguments capture features, etc., because when a nested function is exported to a closure,
    ///     there is a requirement to capture and maintain the local scope at that time.
    ///     Local variables that occurred after the time of capture should have no relation to the
    ///     nested function.
    ///
    ///     in order to implement these functions quickly and effectively, a map container that
    ///     remembers the insertion order is required.
    ///
    ///     the core ideas come from Oliver Schönrock and several great developers at bit.ly/41CwjLL
    template <typename K, typename V> class smultimap {
        NM(ME(smultimap))

    public:
        class wrap;
        typedef std::unordered_multimap<std::string, wrap> stlMap;
        using stlMapPAir = typename stlMap::value_type;

        class wrap {
            friend class smultimap<K, V>;

        public:
            wrap() = default;
            wrap(V&& newValue);
            wrap(const wrap& rhs);
            wrap(wrap&& rhs);

        public:
            wrap& operator=(wrap&);
            wrap& operator=(wrap&&);

        public:
            const K& getKey() const;
            V& getVal();
            const V& getVal() const;

        public:
            void clear();

        private:
            const K* _key = nullptr;
            V _value;
            wrap* _prev = this;
            wrap* _next = this;
        };

        class iterator {
        public:
            iterator(me* owner, wrap* pair);
            friend class smultimap<K, V>;

        public:
            V& operator*();
            V* operator->();

            virtual iterator& operator++();
            virtual iterator& operator--();

            bool isEnd() const;

            const K& getKey() const;
            const V& getVal() const;
            V& getVal();

            bool operator!=(const iterator& rhs) const;
            bool operator==(const iterator& rhs) const;

        private:
            smultimap<K, V>* _owner;
            wrap* _wrap;
        };

        class filteredIterator: public iterator {
        public:
            filteredIterator(me* owner, wrap* pair, const K& key);

        public:
            iterator& operator++() override;

        private:
            const K* _key;
        };

    public:
        smultimap() = default;

    public:
        ncnt size() const;

        iterator begin();
        iterator end();
        filteredIterator begin(const K& key);

        void insert(const K& key, V&& val);
        void erase(const K& key);
        void erase(const iterator& it);

        void clear();

    private:
        typename stlMap::iterator _erase(const typename stlMap::iterator& e);
        void _link(wrap& newTail);
        void _unlink(wrap& toDelete);

    private:
        stlMap _map;
        wrap _end;
    };
} // namespace nm
