/// @file
#pragma once

#include <map>

#include "memlite/interface/clonable.hpp"
#include "memlite/interface/id.hpp"

struct byeolTest;

namespace by {

    class bindTag;
    class instancer;

    /// @ingroup memlite
    /// @brief Base class for all managed instances in memlite system
    /// @details Provides unique identification, type information, and cloning capabilities
    /// for objects managed by the memlite memory management system.
    class _nout instance: public typeProvidable, public clonable {
        BY_ME(instance)
        BY_INIT_META(me)
        friend class bindTag;
        friend class watcher;      // for vault.
        friend class chunks;       // for vault.
        friend class instancer;    // for _id.
        friend struct ::byeolTest; // for vault.

    public:
        class vault {
        public:
            void set(void* ptr, nidx chkN);
            nidx get(void* ptr);
            ncnt len() const;
            nbool rel();
            std::map<void*, int>& getVaults();

        private:
            std::map<void*, int> _vaults;
        };

        //  instance:
        instance();
        explicit instance(id newId);
        instance(const me& rhs);
        virtual ~instance();

    public:
        me& operator=(const me& rhs) = default;
        void* operator new(size_t sz) noexcept;
        void operator delete(void* pt, size_t sz) noexcept;

    public:
        virtual id getId() const;
        virtual nbool isHeap() const;
        const bindTag* getBindTag() const;

    protected:
        //  instance:
        nbool _setId(id new1);
        static instancer* _getMgr();

    private:
        id _id;
        static vault _vault;
    };
} // namespace by
