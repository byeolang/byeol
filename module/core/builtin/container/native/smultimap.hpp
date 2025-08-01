/// @file
#pragma once

#include <unordered_map>
#include "core/common.hpp"

namespace by {
    /// smultimap:
    ///     a multimap container class that preserves insertion order.
    ///     in C++, a general map operates regardless of the insertion order when iteration is
    ///     performed.
    ///
    ///     this class manages key and value as a pair like multimap, but when iteration is
    ///     performed, it remembers the insertion order and iterates like a queue. This function is
    ///     used in arguments capture features, etc., because when a nested function is exported to
    ///     a closure, there is a requirement to capture and maintain the local scope at that time.
    ///     Local variables that occurred after the time of capture should have no relation to the
    ///     nested function.
    ///
    ///     in order to implement these functions quickly and effectively, a map container that
    ///     remembers the insertion order is required.
    ///
    ///     the core ideas come from Oliver Schönrock and several great developers at bit.ly/41CwjLL
    template <typename K, typename V> class smultimap {
        BY(ME(smultimap))

    public:
        class wrap;
        class iterator;
        friend class iterator;
        typedef std::unordered_multimap<K, wrap> stlMap;

        class wrap {
            friend class smultimap<K, V>;

        public:
            wrap() = default;
            wrap(V&& newValue);
            wrap(const wrap& rhs);
            wrap(wrap&& rhs);

        public:
            wrap& operator=(const wrap&);
            wrap& operator=(wrap&&);

        public:
            const K* getKey() const;
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
            BY(ME(iterator))
            typedef smultimap<K, V> owner;

        public:
            iterator(const smultimap* owner, const wrap* pair, nbool isReversed);
            iterator(const smultimap* owner, const wrap* pair, nbool isReversed, const K& key);
            iterator(const smultimap* owner, const wrap* pair, nbool isReversed, const K* key);
            friend owner;

        public:
            V& operator*();
            V* operator->();

            iterator& operator++();
            iterator operator++(int);
            iterator& operator--();
            iterator operator--(int);
            iterator operator+(ncnt step);

            bool isEnd() const;

            const K* getKey() const;
            const V* getVal() const BY_CONST_FUNC(getVal())
            V* getVal();

            bool operator!=(const iterator& rhs) const;
            bool operator==(const iterator& rhs) const;

        private:
            iterator& _step(ncnt step, nbool isReversed);
            static const K& _getDummyKey();

        private:
            const owner* _owner;
            const wrap* _wrap;
            nbool _isReversed;
            K _key;
        };

    public:
        smultimap() = default;

    public:
        ncnt size() const;

        iterator begin() const;
        iterator end() const;
        iterator begin(const K& key) const;
        iterator begin(const K* key) const BY_SIDE_FUNC(key, begin(*key), begin());

        iterator rbegin() const;
        iterator rend() const;
        iterator rbegin(const K& key) const;
        iterator rbegin(const K* key) const BY_SIDE_FUNC(key, rbegin(*key), rbegin());

        void insert(const K& key, V&& val);

        /// delete all elements matching given key.
        void erase(const K& key);
        void erase(const K* it) BY_SIDE_FUNC(erase);
        void erase(const iterator& it);
        void erase(const iterator* it) BY_SIDE_FUNC(erase);
        void erase(const iterator& from, const iterator& to);
        void erase(const iterator* from, const iterator& to) BY_SIDE_FUNC(from, erase(*from, to), void());
        void erase(const iterator& from, const iterator* to) BY_SIDE_FUNC(to, erase(from, *to), void());
        void erase(const iterator* from, const iterator* to) BY_SIDE_FUNC(from&& to, erase(*from, *to), void());

        iterator find(const K& key) const;

        void clear();

    private:
        typename stlMap::iterator _erase(const typename stlMap::iterator& e);
        void _erase(const iterator& e);
        void _link(wrap& newTail);
        void _unlink(wrap& toDelete);
        iterator _begin(const K& key, nbool isReversed) const;

    private:
        stlMap _map;
        wrap _end;
    };
} // namespace by
