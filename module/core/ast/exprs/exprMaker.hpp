/// @file
#pragma once

#include "core/ast/expr.hpp"
#include "core/common/coreInternal.hpp"

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
            if(_file) coreInternal::setSrc(*ret, *makeSrc(name));
            return ret;
        }

        /**
         * @brief Creates a new expression object without a specific name and sets its source location.
         * @tparam T The type of the expression to create.
         * @tparam Args The types of the constructor arguments for T.
         * @param args The constructor arguments for T.
         * @return A pointer to the newly created expression object.
         */
        template <typename T, typename... Args> T* make(const Args&... args) const {
            T* ret = new T(args...);
            if(_file) coreInternal::setSrc(*ret, *makeSrc(""));
            return ret;
        }

        /**
         * @brief Creates a function specifically for handling "expand" logic within definition blocks.
         * @param blk The definition block associated with the expand function.
         * @return A pointer to the newly created `func` object.
         */
        func* makeExpandFunc(const defBlock& blk) const;
        /**
         * @brief Creates an assignment expression from a variable definition expression.
         * @param e The variable definition expression (`defVarExpr`) to derive the assignment from.
         * @return A pointer to the newly created `assignExpr` object.
         */
        assignExpr* makeAssignExprFrom(const defVarExpr& e) const;
        /**
         * @brief Creates a "common" function associated with a definition block.
         * @param blk The definition block for which the common function is being created.
         * @return A pointer to the newly created `func` object.
         */
        func* makeCommonFunc(const defBlock& blk) const;

    private:
        func* _makeFunc(const std::string& name, const narr& stmts) const;

    private:
        tstr<srcFile> _file;
        point _pos;
    };
} // namespace by
