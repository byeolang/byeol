/// @file
#pragma once

#include "core/ast/expr.hpp"

namespace by {

    class func;
    class defBlock;
    class assignExpr;
    class defVarExpr;

    /** @ingroup core
     *  @brief Factory for creating expressions with source location information
     *  @details Creates expression objects with proper source file and position metadata.
     *  Used by parser to track source locations for debugging and error reporting.
     */
    class _nout exprMaker: public typeProvidable, public clonable {
        BY(CLASS(exprMaker))

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

        tstr<src> makeSrc(const std::string& name) const;

        /**
         *  @note when you birth your child, you must name it.
         */
        template <typename T, typename... Args> T* birth(const std::string& name, const Args&... args) const {
            T* ret = new T(args...);
            if(_file) ret->_setSrc(*makeSrc(name));
            return ret;
        }

        template <typename T, typename... Args> T* make(const Args&... args) const {
            T* ret = new T(args...);
            if(_file) ret->_setSrc(*makeSrc(""));
            return ret;
        }

        func* makeExpandFunc(const defBlock& blk) const;
        assignExpr* makeAssignExprFrom(const defVarExpr& e) const;
        func* makeCommonFunc(const defBlock& blk) const;

    private:
        func* _makeFunc(const std::string& name, const narr& stmts) const;

    private:
        tstr<srcFile> _file;
        point _pos;
    };
} // namespace by
