/// @file
#pragma once

#include "core/ast/exprs/endExpr.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/ast/exprs/blockExpr.hpp"
#include "core/ast/params.hpp"
#include "core/ast/scope.hpp"
#include "core/type/funcType.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Function definition in byeol language
     *  @details Represents a user-defined function with parameters, return @ref ntype "type", and @ref blockExpr
     *  "executable block". Extends @ref baseFunc to provide concrete function implementation with actual code body.
     *  Manages function execution context, parameter handling, and return value processing.
     *
     *  @section function_execution Function Execution
     *  When a func is evaluated:
     *  1. Creates a new @ref frame and registers it with @ref thread
     *  2. Binds arguments to parameters in the frame's scope
     *  3. Executes the @ref blockExpr body
     *  4. Processes return value and cleans up the frame
     *  5. Runs any @ref endExpr statements (cleanup code)
     *
     *  @section frame_interaction Frame Interaction
     *  func implements @ref frameInteractable, meaning it interacts with the execution frame stack. During execution,
     *  it calls `inFrame()` to register its scope and parameters, then `outFrame()` to clean up when execution
     *  completes.
     *
     *  @section abstract_functions Abstract Functions
     *  func can be abstract (no body) when used in type definitions. Use `isAbstract()` to check. Abstract functions
     *  have no @ref blockExpr and cannot be executed directly.
     */
    class _nout func: public baseFunc {
        BY(ME(func, baseFunc), CLONE(func), VISIT())
        typedef std::function<void(const std::string&, const node&)> onEval;
        friend class coreInternal;

    public:
        explicit func(const modifier& mod, const funcType& type);
        explicit func(const modifier& mod, const funcType& type, const blockExpr& newBlock);
        explicit func(const modifier& mod, const funcType& type, const scope& subs, const blockExpr& newBlock);

    public:
        const ntype& getType() const override;

        blockExpr& getBlock();
        const blockExpr& getBlock() const;
        void setBlock(const blockExpr& new1);
        scope& subs() override;

        using super::eval;
        str eval(const args& a) override;

        using super::inFrame;
        void inFrame(const bicontainable* args) const override;

        using super::outFrame;
        void outFrame() const override;

        const baseObj& getOrigin() const override;

        /**
         * @brief Gets the list of `endExpr` (defer-like statements) registered for this function.
         * @return A reference to the `ends` container holding the `endExpr` objects.
         */
        ends& getEnds();
        const ends& getEnds() const BY_CONST_FUNC(getEnds())

        void onCloneDeep(const clonable& from) override;

        nbool isAbstract() const;

    protected:
        /**
         * @brief Protected virtual method to interact with the execution frame during function evaluation.
         * @details This method is a crucial internal mechanism for setting up the function's execution
         *          context within the current frame, managing `me` object and local scope.
         * @param meObj The "me" object (context) for this function call.
         * @param s The scope associated with this function.
         * @param exN An index related to exception handling.
         * @return An `str` object representing the result of the frame interaction.
         */
        virtual str _interactFrame(node& meObj, scope& s, nidx exN);
        str _run(nidx exN);

    private:
        scope* _evalArgs(const args& args);
        str _postprocess(const str& ret, nidx exN);
        void _runEnds();

    public:
        // special funcs:
        inline static const std::string LAMBDA_NAME = "@lambda";
        inline static const std::string CTOR_NAME = "@ctor";
        inline static const std::string GETTER_NAME = "@get";
        inline static const std::string SETTER_NAME = "@set";
        inline static const std::array<std::string, 4> SPECIAL_FUNC_NAMES = {LAMBDA_NAME, CTOR_NAME, GETTER_NAME,
            SETTER_NAME};

    private:
        tstr<obj> _org;
        funcType _type;
        scope _subs;
        tstr<blockExpr> _blk;
        ends _ends;
    };
} // namespace by
