/// @file
#pragma once

#include "core/builtin/err/errReport.hpp"
#include "core/ast/manifest.hpp"
#include "core/ast/origin.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Pack slot
     *  @details The byeol language distributes libraries in units called `pack`, a type of compressed file. A pack
     *  file contains a top-level `@ref obj` object, @ref manifest, and a list of dependent packs. (In byeol, to use
     *  another pack in your code, you must declare the dependency relationship in the manifest.)
     *
     *  A slot is the result created from a pack file; it doesn't handle loading the pack. See @ref packLoading or
     *  @ref slotLoader for pack loading details.
     */
    class _nout slot: public node {
        BY(CLASS(slot, node), VISIT())

    public:
        slot(const manifest& manifest);
        slot(const manifest& manifest, const obj& pack);

    public:
        manifest& getManifest();
        const manifest& getManifest() const;
        nbool isValid() const;
        /**
         * @brief Retrieves the top-level object (`obj`) of this pack.
         * @return A reference to the pack's `obj`.
         */
        virtual obj& getPack();
        const obj& getPack() const BY_CONST_FUNC(getPack())
        void rel() override;

        /**
         * @brief Adds a dependent slot to this slot.
         * @param dependent The slot that depends on this one.
         */
        void addDependent(me& dependent);
        void addDependent(me* it) BY_SIDE_FUNC(addDependent)

            const tnarr<me>& getDependents() const;

        using super::subs;
        scope& subs() override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        using super::eval;
        str eval(const args& a) override;

        str infer() const override;

    protected:
        /**
         * @brief Protected virtual method to invalidate the slot.
         * @details This method is typically called internally to mark the slot as invalid,
         *          making it unusable for further operations.
         * @return true if the slot was successfully invalidated, false otherwise.
         */
        virtual nbool _invalidate();
        void _setValid(nbool valid);
        str _onEvalSub(node& sub, const args& a) override;

    private:
        void _rel();

    protected:
        tstr<obj> _pak;

    private:
        manifest _manifest;
        nbool _isValid;
        tnarr<me> _dependents;
    };

    typedef tnmap<std::string, slot> slots;
    typedef tnchain<std::string, slot> slotChain;
    typedef tnbicontainer<std::string, slot> slotContainer;
} // namespace by
