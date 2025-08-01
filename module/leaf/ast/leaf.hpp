/// @file
#pragma once

#include "leaf/common.hpp"

namespace by {

    /// @ingroup leaf
    /// @brief Core AST node for leaf configuration language
    /// @details Represents a node in the leaf configuration tree structure.
    /// Supports hierarchical configuration data with key-value access patterns.
    class _nout leaf: public instance {
        BY(ME(leaf, instance), CLONE(me), INIT_META(leaf))

        typedef std::map<std::string, tstr<me>> myMap;
        typedef myMap::iterator iterator;

    public:
        leaf(std::initializer_list<me*> subs, const std::string& name = "");
        leaf(const me& rhs, const std::string& name = "");
        leaf(const std::string& name = "");

    public:
        me& operator[](const std::string& name);
        const me& operator[](const std::string& name) const BY_CONST_FUNC(operator[](name));
        me& operator[](const nchar* name);
        const me& operator[](const nchar* name) const BY_CONST_FUNC(operator[](name));
        explicit operator nbool() const;

    public:
        me& sub(const std::string& name);
        me& sub(const std::string* it) BY_SIDE_FUNC(it, sub(*it), *this);
        const me& sub(const std::string& name) const BY_CONST_FUNC(sub(name))
        const me& sub(const std::string* name) const BY_CONST_FUNC(sub(name))
        me& sub(const nchar* name);
        const me& sub(const nchar* name) const BY_CONST_FUNC(sub(name))
        me& sub(nidx n);
        const me& sub(nidx n) const BY_CONST_FUNC(sub(n))

        nbool has(const std::string& name) const;
        nbool has(const std::string* it) const BY_SIDE_FUNC(has);

        void add(const leaf& it);
        void add(const leaf* it) BY_SIDE_FUNC(add);
        void add(std::initializer_list<leaf*> subs);
        void del(const leaf& it);
        void del(const leaf* it) BY_SIDE_FUNC(del);
        void del(const std::string& name);
        void del(const std::string* it) BY_SIDE_FUNC(del);
        void del(const nchar* name);

        const std::string& getName() const;
        void setName(const std::string& newName);
        void setName(const std::string* it) BY_SIDE_FUNC(setName);
        void setName(const nchar* newName);

        ncnt len() const;

        iterator begin();
        iterator end();

        const type& getType() const override;

        virtual nint asInt() const;
        virtual nchar asChar() const;
        virtual const std::string& asStr() const;
        virtual nbool asBool() const;
        virtual nbool isExist() const;

    private:
        myMap _subs;
        std::string _name;
    };
} // namespace by
