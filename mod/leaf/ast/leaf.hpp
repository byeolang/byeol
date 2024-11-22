#pragma once

#include "../common.hpp"

namespace nm {

    class _nout leaf: public instance {
        NM(ME(leaf, instance), CLONE(me), INIT_META(leaf))

        typedef std::map<std::string, tstr<me>> myMap;
        typedef myMap::iterator iterator;

    public:
        leaf(std::initializer_list<me*> subs, const std::string& name = "");
        leaf(const me& rhs, const std::string& name = "");
        leaf(const std::string& name = "");

    public:
        me& operator[](const std::string& name);
        const me& operator[](const std::string& name) const NM_CONST_FUNC(operator[](name))
        me& operator[](nidx n);
        const me& operator[](nidx n) const NM_CONST_FUNC(operator[](n))

        explicit operator nbool() const;

        me& sub(const std::string& name);
        const me& sub(const std::string& name) const NM_CONST_FUNC(sub(name))

        nbool has(const std::string& name) const;

        void add(const leaf& new1);
        void add(std::initializer_list<leaf*> subs);
        void del(const leaf& it);
        void del(const std::string& name);

        const std::string& getName() const;
        void setName(const std::string& newName);

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
} // namespace nm
