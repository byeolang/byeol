/// @file
#pragma once

#include "stela/common.hpp"

namespace by {

    /** @ingroup stela
     *  @brief Core AST node for stela configuration language
     *  @details The fundamental unit class of the stela module, providing the following
     *  features:
     *
     *  1. Provides type conversion APIs like asInt(), asChar() to int, char, string, bool
     *  2. When value exists, @ref valStela APIs execute, converting to appropriate types.
     *     For example, verStela(22).asStr() returns std::string("22")
     *  3. When no value exists, represented as @ref nulStela. Any type conversion attempt
     *     returns default value (empty string or 0)
     *  4. Provides version type with major, minor, patch versions, supporting range
     *     expressions
     *  5. stela can have other stela objects as children. Each stela has a name, so children
     *     are searched by name or traversed. If no child matches the name, nulStela is returned
     *
     *  Similar to @ref node in the core module.
     *
     *  @section usage Usage
     *  Example demonstrates typical usage:
     *  @code
     *      const std::string script = R"SRC(
     *          def man
     *              dummy := 5
     *              name := "dark souls"
     *              ver := 1.0.8
     *      )SRC";
     *
     *      root = stelaParser().parse(script);
     *      ASSERT_TRUE(root);
     *
     *      stela& man = root->sub("man");
     *      stela& name = man["name"];
     *      ASSERT_TRUE(name);
     *
     *      ASSERT_STREQ(name.asStr().c_str(), "dark souls");
     *
     *      verStela& ver = man["ver"].cast<verStela>() OR_ASSERT(ver);
     *      ASSERT_STREQ(ver.asStr().c_str(), "1.0.8");
     *      ASSERT_EQ(ver.asMajor(), 1);
     *      ASSERT_EQ(ver.asMinor(), 0);
     *      ASSERT_EQ(ver.asFix(), 8);
     *  @endcode
     */
    class _nout stela: public instance {
        BY(ME(stela, instance), CLONE(me), INIT_META(stela))

        typedef std::map<std::string, tstr<me>> myMap;
        typedef myMap::iterator iterator;

    public:
        /**
         * @brief Constructs a stela object with an optional name and an initializer list of child stela objects.
         * @param subs An initializer list of stela pointers to be added as children.
         * @param name The name of this stela object.
         */
        stela(std::initializer_list<me*> subs, const std::string& name = "");
        stela(const me& rhs, const std::string& name = "");
        stela(const std::string& name = "");

    public:
        me& operator[](const std::string& name);
        const me& operator[](const std::string& name) const BY_CONST_FUNC(operator[](name));
        me& operator[](const nchar* name);
        const me& operator[](const nchar* name) const BY_CONST_FUNC(operator[](name));
        /**
         * @brief Converts the stela object to a boolean, indicating its existence.
         * @return true if the stela object is valid and exists (i.e., not a @ref nulStela), false otherwise.
         */
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

        /**
         * @brief Adds a stela object as a child.
         * @param it The stela object to add as a child.
         */
        void add(const stela& it);
        /**
         * @brief Adds a stela object as a child.
         * @param it Pointer to the stela object to add as a child.
         */
        void add(const stela* it) BY_SIDE_FUNC(add);
        /**
         * @brief Adds multiple stela objects from an initializer list as children.
         * @param subs An initializer list of stela pointers to add as children.
         */
        void add(std::initializer_list<stela*> subs);
        void del(const stela& it);
        void del(const stela* it) BY_SIDE_FUNC(del);
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

        /**
         * @brief Converts the stela object's value to an integer.
         * @details If the stela object has no value or is a @ref nulStela, it returns 0.
         *          If the conversion from its internal type (e.g., string) fails, an exception may be thrown.
         * @return The integer representation of the stela object's value.
         */
        virtual nint asInt() const;
        /**
         * @brief Converts the stela object's value to a character.
         * @details If the stela object has no value or is a @ref nulStela, it returns a null character ('\\0').
         *          If the conversion from its internal type (e.g., string) fails, an exception may be thrown.
         * @return The character representation of the stela object's value.
         */
        virtual nchar asChar() const;
        /**
         * @brief Converts the stela object's value to a string.
         * @details If the stela object has no value or is a @ref nulStela, it returns an empty string.
         * @return A const reference to the string representation of the stela object's value.
         */
        virtual const std::string& asStr() const;
        /**
         * @brief Converts the stela object's value to a boolean.
         * @details If the stela object has no value or is a @ref nulStela, it returns false.
         *          If the conversion from its internal type (e.g., string) fails, an exception may be thrown.
         * @return The boolean representation of the stela object's value.
         */
        virtual nbool asBool() const;
        /**
         * @brief Checks if the stela object is considered "existent" (i.e., not a @ref nulStela).
         * @return true if the stela object holds a value, false if it represents a null or non-existent entry.
         */
        virtual nbool isExist() const;

    private:
        myMap _subs;
        std::string _name;
    };
} // namespace by
