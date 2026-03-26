/// @file
#pragma once

#include "core/ast/params.hpp"
#include "core/type/ntype.hpp"

namespace by {

    class node;
    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<node, strTactic> narr;

    /** @ingroup core
     *  @brief Script type for byeol objects
     *  @details Type information for scripted objects with inheritance, parameters, and templates.
     *  Handles complex type relationships and runtime type information.
     */
    class _nout mgdType: public ntype, public clonable {
        BY(ME(mgdType, ntype), CLONE(mgdType))

    public:
        mgdType(const std::string& name, const type& super, const params& ps);
        /**
         * @brief Constructs a script type with name, super type, parameters, ADT status, and return node.
         * @param name The name of the script type.
         * @param super The super type for inheritance.
         * @param ps The parameters associated with this type.
         * @param isAdt A boolean indicating if this type represents an Algebraic Data Type (ADT).
         * @param ret The return node associated with this type (e.g., for functions).
         */
        mgdType(const std::string& name, const type& super, const params& ps, nbool isAdt, const node* ret);
        mgdType(const std::string& name, const types& supersFromRhs);

    private:
        /**
         * @brief Protected constructor for internal use, defining a script type with full hierarchy information.
         * @param name The name of the script type.
         * @param supers A list of super types for inheritance.
         * @param subs A list of sub types.
         * @param ps The parameters associated with this type.
         * @param isAdt A boolean indicating if this type represents an Algebraic Data Type (ADT).
         */
        mgdType(const std::string& name, const types& supers, const types& subs, const params& ps, nbool isAdt);

    public:
        nbool isTemplate() const override;
        nbool isAbstract() const override;
        const type& getSuper() const override;
        const nbool& isInit() const override;
        const std::string& getName() const override;

        ncnt size() const override;

        /**
         * @brief Performs a deep clone operation for this script type instance.
         * @details This method ensures that all components of the script type, including its
         *          internal structures and parameters, are deeply copied from the source.
         * @param from The clonable object to copy from.
         */
        void onCloneDeep(const clonable& from) override;

        using super::getParams;
        params& getParams() override;

        const node* getRet() const override;

        using super::setRet;
        void setRet(const node& new1) override;

        // TODO: getSubs(), getLeafs()

        void* make() const override;

    protected:
        types& _getSupers() override;
        types& _getSubs() override;
        type& _getStatic() const override;
        types** _onGetLeafs() const override;

    private:
        void _initSupers(const type& super);

    private:
        std::string _name;
        types _supers;
        types _subs;
        params _params;
        nbool _isAdt;
        str _ret;
    };
} // namespace by
