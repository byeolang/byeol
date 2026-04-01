/// @file
#pragma once

#include "core/builtin/err/errReport.hpp"
#include "core/ast/manifest.hpp"
#include "core/ast/origin.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Pack
     *  @details The byeol language distributes libraries in units called `pack`, a type of compressed file. A pack
     *  file contains a top-level `@ref obj` object, @ref manifest, and a list of dependent packs. (In byeol, to use
     *  another pack in your code, you must declare the dependency relationship in the manifest.)
     *
     *  A pack is the result created from a pack file; it doesn't handle loading the pack. See @ref packLoading or
     *  @ref packLoader for pack loading details.
     */
    class _nout pack: public obj {
        BY(CLASS(pack, obj), VISIT())

    public:
        pack(const manifest& mani);

    public:
        manifest& getManifest();
        const manifest& getManifest() const;
        nbool isValid() const;
        void rel() override;

        /**
         * @brief Adds a dependent pack to this slot.
         * @param dependent The pack that depends on this one.
         */
        void addDependent(me& dependent);
        void addDependent(me* it) BY_SIDE_FUNC(addDependent);

        const tnarr<me>& getDependents() const;

    protected:
        /**
         * @brief Protected virtual method to invalidate the pack.
         * @details This method is typically called internally to mark the pack as invalid,
         *          making it unusable for further operations.
         * @return true if the pack was successfully invalidated, false otherwise.
         */
        virtual nbool _invalidate();
        void _setValid(nbool valid);

    private:
        void _rel();

    private:
        manifest _manifest;
        nbool _isValid;
        tnarr<me> _dependents;
    };

    typedef tnmap<std::string, pack> packs;
    typedef tnchain<std::string, pack> packChain;
    typedef tnbicontainer<std::string, pack> packContainer;
} // namespace by
