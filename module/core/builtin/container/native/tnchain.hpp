#pragma once

#include "core/builtin/container/native/tnmap.hpp"

namespace by {

    class node;

    template <typename K, typename V, typename _defaultContainer = tnmap<K, V>>
    class tnchain: public tnbicontainer<K, V> {
        typedef tnbicontainer<K, V> _super_;
        typedef tnchain<K, V, _defaultContainer> _me_;
        BY(CLASS(_me_, _super_))

    public:
        typedef typename super::iter iter;
        typedef typename super::iteration iteration;
        friend class nchainIteration;
        typedef _defaultContainer defaultContainer;
#include "core/builtin/container/iter/nchainIteration.hpp"

    public:
        tnchain();
        explicit tnchain(const super& arr);
        explicit tnchain(const super& org, const me& next);
        explicit tnchain(std::initializer_list<std::pair<K, V*>> elems);

    public:
        // has:
        using super::in;
        nbool in(const K& key) const override;

        // len:
        ncnt len() const override;
        ncnt chainLen() const;

        using super::get;
        V* get(const K& key) override;

        // add:
        using super::add;
        nbool add(const K& key, const V& val) override;

        /// @brief link to another chain instance.
        ///        if iterator is not the begin of another chain,
        ///        this chain won't link unaccessible elements via given iterator.
        ///        using this point, you can choose which elements to link and which elements not.
        ///        even if you specified a reverse iterator to link, this chain will only traverse
        ///        in reverse way for that container refered by the reversed iterator.
        /// @param portion this iterator should be belong to another chain instance.
        ///                 of course you can input this iterator with reversed one.
        ///                 if this iterator reached to the end, chain iterator will be forward to
        ///                 next chain instance.
        /// @return false when to make a link didn't work.
        ///               for instance, if iterator is belonged to this chain or nullptr.
        virtual nbool link(const iter& portion);
        nbool link(const me& new1);
        nbool link(const me* it) BY_SIDE_FUNC(link);
        nbool unlink();

        // del:
        using super::del;
        /// delete all elements matching by given key.
        nbool del(const K& key) override;
        nbool del(const iter& at) override;
        nbool del(const iter& from, const iter& end) override;

        // etc:
        void rel() override;

        super& getContainer();
        const super& getContainer() const;

        me* getNext();
        const me* getNext() const BY_CONST_FUNC(getNext())

        me* getPrev();
        const me* getPrev() const BY_CONST_FUNC(getPrev())

        /// return most not null next element of this chain.
        me* getTail();
        const me* getTail() const BY_CONST_FUNC(getTail())

        /// returned deep cloned of this instance with all chained ones.
        /// @remark when the chain has already linked to the another chain instance,
        ///         not only this object, but also all of chained instances will be deep cloned.
        void onCloneDeep(const clonable& from) override;

        /// wrap given container as the same level to this chain.
        /// @param toShallowWrap
        ///        if this is a chain, then the wrap func returns it as it is.
        ///        if this is any container except chain, then it returns after
        ///        wrapping given container.
        template <typename T> static T* wrap(const super& toShallowWrap) {
            T* ret = (T*) toShallowWrap.template cast<T>();
            if(!ret) {
                ret = new T();
                ret->_map.bind(toShallowWrap);
            }

            return ret;
        }

        template <typename T>
        static T* wrap(const super* it) BY_SIDE_FUNC(it, wrap<T>(*it), nullptr);

        static me* wrap(const super& toShallowWrap);
        static me* wrap(const super* it) BY_SIDE_FUNC(wrap);

        /// mock this chain and let it chain another container differ to original.
        /// this func keep accessing next element to chain it.
        /// @param until the loop will be terminated when next element has same address to this.
        ///        chain you assigned to 'until' will be cloned to.
        me* cloneChain(const super* until) const;
        me* cloneChain(const me* until) const;

        /// mock all of this chain until 'next' is null.
        me* cloneChain() const;

    protected:
        iteration* _onMakeIteration(const K* key, nbool isReversed, ncnt step,
            nbool isBoundary) const override;

        void _getAll(const K& key, narr& tray) const override;

    private:
        iter* _getInnerIter(const iter& wrapper);
        iter _getInnerBeginOfChain(me& it, const me& fromChain, const iter& from);
        iter _getInnerEndOfChain(me& it, const me& lastChain, const iter& last);
        iter _rendOfThisChain(nbool isReversed);
        me& _getLastChain();
        void _eachChain(std::function<void(me&)> closure);

    private:
        tstr<super> _map;
        iter _next;
        iter _prev;
    };

    extern template class _nout tnchain<std::string, node, tnmap<std::string, node, strTactic>>;
    typedef tnchain<std::string, node, tnmap<std::string, node, strTactic>> nchain;
} // namespace by
