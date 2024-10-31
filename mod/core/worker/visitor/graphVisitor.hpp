#pragma once

#include "visitor.hpp"

namespace nm {

    class _nout graphVisitor: public visitor {
        NM(CLASS(graphVisitor, visitor))

    public:
        nbool onVisit(const visitInfo& i, node& visitee) override;
        void onLeave(const visitInfo& i, node& visitee) override;
        nbool onVisit(const visitInfo& i, baseFunc& fun) override;
        nbool onVisit(const visitInfo& i, genericOrigin& o) override;
        nbool onVisit(const visitInfo& i, getExpr& e) override;
        nbool onVisit(const visitInfo& i, runExpr& e) override;
        nbool onVisit(const visitInfo& i, FBOExpr& e) override;
        nbool onVisit(const visitInfo& i, FUOExpr& e) override;
        nbool onVisit(const visitInfo& i, assignExpr& e) override;
        nbool onVisit(const visitInfo& i, defVarExpr& e) override;
        nbool onVisit(const visitInfo& i, nInt& e) override;
        nbool onVisit(const visitInfo& i, nFlt& e) override;
        nbool onVisit(const visitInfo& i, nStr& e) override;
        nbool onVisit(const visitInfo& i, nByte& e) override;
        nbool onVisit(const visitInfo& i, nBool& e) override;

    protected:
        void _onWork() override;

    private:
        void _drawIndent();
        void _onIndent();
        void _drawFrame(const visitInfo& i);

        template <typename T> nbool _onVisitPrimitive(const visitInfo& i, T& e) {
            _drawFrame(i);
            using platformAPI::foreColor;
            std::cout << foreColor(LIGHTRED) << i.name << " " << foreColor(CYAN)
                      << e.getType().getName() << foreColor(LIGHTGRAY) << " = " << foreColor(YELLOW)
                      << e.get();
            return false;
        }

        std::string _encodeNewLine(const std::string& msg) const;
        std::string _getNameFrom(const node& it) const;

    private:
        std::vector<nbool> _parentsLast;
        std::vector<const char*> _indents;
        nbool _isStart;
    };
} // namespace nm
