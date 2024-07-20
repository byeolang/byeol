#pragma once

#include "srcFile.hpp"

namespace nm {
    class _nout src : public instance {
        NM(CLASS(src, instance))
        friend class parser;
        friend class genericOrigin; // for _setName() at genericOrigin::_makeGeneric()

    public:
        src(const srcFile& file, const std::string& name, const point& pos);

    public:
        const srcFile& getFile() const;
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
