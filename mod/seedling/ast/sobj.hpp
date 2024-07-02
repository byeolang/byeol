#pragma once

#include "../common.hpp"

namespace nm {

    class _nout sobj : public instance {
        NAMU(ME(sobj, instance),
            CLONE(me),
            INIT_META(sobj))

        typedef std::map<std::string, tstr<me>> myMap;
        typedef myMap::iterator iterator;

    public:
        sobj(std::initializer_list<me*> subs, const std::string& name = "");
        sobj(const me& rhs, const std::string& name = "");
        sobj(const std::string& name = "");

    public:
        me& operator[](const std::string& name);
        const me& operator[](const std::string& name) const NAMU_UNCONST_FUNC(operator[](name))
        me& operator[](nidx n);
        const me& operator[](nidx n) const NAMU_UNCONST_FUNC(operator[](n))

        explicit operator nbool() const;

        me& sub(const std::string& name);
        const me& sub(const std::string& name) const NAMU_UNCONST_FUNC(sub(name))

        nbool has(const std::string& name) const;

        void add(const sobj& new1);
        void add(std::initializer_list<sobj*> subs);
        void del(const sobj& it);
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
}
