#pragma once

#include "../expr.hpp"

namespace nm {

    class _nout exprMaker: public typeProvidable, public clonable {
        NM(CLASS(exprMaker))

    public:
        exprMaker();
        exprMaker(const srcFile& s, ncnt row, ncnt col);

    public:
        me& addRow();
        me& addRow(ncnt step);
        me& addCol();
        me& addCol(ncnt step);

        me& setRow(ncnt row);
        me& setCol(ncnt col);
        me& setSrcFile(const srcFile& s);

        void rel();

        const point& getPos() const;
        const srcFile& getSrcFile() const;

        /// when you birth your child, you must name it.
        template <typename T, typename... Args>
        T* birth(const std::string& name, const Args&... args) const {
            T* ret = new T(args...);
            if(_file) ret->_setSrc(*new src(*_file, name, _pos));
            return ret;
        }

        template <typename T, typename... Args> T* make(const Args&... args) const {
            T* ret = new T(args...);
            if(_file) ret->_setSrc(*new src(*_file, "", _pos));
            return ret;
        }

    private:
        tstr<srcFile> _file;
        point _pos;
    };
} // namespace nm
