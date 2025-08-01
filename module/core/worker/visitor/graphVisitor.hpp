/// @file
#pragma once

#include "core/worker/visitor/visitor.hpp"

namespace by {

    /// @ingroup core
    /// @brief AST graph visualization visitor
    /// @details Visitor that generates tree-like visual representations of AST nodes for debugging.
    class _nout graphVisitor: public visitor {
        BY(CLASS(graphVisitor, visitor))

    public:
        graphVisitor();

    public:
        nbool onVisit(const visitInfo& i, node& visitee, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, node& visitee, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, obj& obj, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, baseFunc& fun, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, func& fun, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, defNestedFuncExpr& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, genericOrigin& o, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, getExpr& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, runExpr& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, FBOExpr& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, FUOExpr& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, assignExpr& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, defVarExpr& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, nInt& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, nFlt& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, nStr& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, nByte& e, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, nBool& e, nbool alreadyVisited) override;
        me& setShowData(nbool showData);
        nbool isShowData() const;

    protected:
        void _onWork() override;

    private:
        void _drawIndent();
        void _onIndent();
        void _drawFrame(const visitInfo& i);

        template <typename T> nbool _onVisitPrimitive(const visitInfo& i, T& e) {
            _drawFrame(i);
            using platformAPI::foreColor;
            _showModifier(e.getModifier());
            std::cout << foreColor(LIGHTRED) << i.name << " " << foreColor(CYAN) << e.getType().getName()
                      << foreColor(LIGHTGRAY) << " = " << foreColor(YELLOW) << e.get();
            return false;
        }

        std::string _encodeNewLine(const std::string& msg) const;
        std::string _getNameFrom(const node& it) const;
        std::string _getNameFrom(const node* it) const BY_SIDE_FUNC(it, _getNameFrom(*it), "frame");
        void _showModifier(const modifier& mod);

    private:
        std::vector<nbool> _parentsLast;
        std::vector<const char*> _indents;
        nbool _isStart;
        nbool _isShowData;
    };
} // namespace by
