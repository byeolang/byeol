/// @file
#pragma once

#include "core/ast/src/srcFile.hpp"

namespace by {
    /** @ingroup core
     *  @brief Source location information
     *  @details Contains which @ref srcFile and at what location within it the created AST @ref node was generated.
     *  Used as a core feature in the byeol language's Context-based REPL.
     *
     *  @remark src information is limited to the managed environment.
     *  Objects created in the native environment have dummy values in src. See @ref dumSrc for details.
     */
    class _nout src: public instance {
        BY(CLASS(src, instance))
        friend class parser;
        friend class genericOrigin; // for _setName() at genericOrigin::_makeGeneric()

    public:
        src(const srcFile& file, const std::string& name, const point& pos);

    public:
        const srcFile* getFile() const;
        const point& getPos() const;
        const std::string& getName() const;

    protected:
        virtual void _setFile(const srcFile& new1);
        virtual void _setPos(const point& new1);
        virtual void _setName(const std::string& new1);

    private:
        tstr<srcFile> _file;
        std::string _name;
        point _pos;
    };

    template <typename T, typename WRP> class tnarr;
    typedef tnarr<src> srcs;
}
