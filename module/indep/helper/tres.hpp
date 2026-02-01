/// @file
#pragma once

#include "tmay.hpp"

namespace by {

    /** @ingroup indep
     *  @brief Template result container with typed error information
     *  @details Identical to tmay but allows defining a custom error type when errors occur.
     *  While tmay<T> can only indicate whether an error occurred or not, tres<T, R> can
     *  provide detailed error information of type R when an error occurs.
     *
     *  @section usage Usage
     *  Example comparing tmay and tres:
     *  @code
     *      // Using tmay - can only tell if error occurred:
     *      tmay<int> result1 = parseNumber("abc");
     *      if(!result1.has()) {
     *          // Know there's an error, but not what kind
     *      }
     *
     *      // Using tres - can get error details:
     *      tres<int, std::string> result2 = parseNumber("abc");
     *      if(!result2.has()) {
     *          std::string errorMsg = result2.getErr(); // Get specific error message
     *          BY_E("Parse failed: %s", errorMsg.c_str());
     *      }
     *  @endcode
     *
     *  Common error types include std::string for error messages, or custom error enums
     *  for categorized error handling.
     */
    template <typename T, typename R> class tres: public tmay<T> {
        typedef tres<T, R> __me__;
        BY(ME(__me__, tmay<T>))

    public:
        tres() = default;
        explicit tres(const T& value);
        explicit tres(const R& value);

    public:
        /**
         * @brief Gets the contained error.
         * @warning Should only be called when has() returns false. Accessing when has()
         *          is true results in undefined behavior.
         * @return A reference to the contained error object.
         */
        R& getErr();
        const R& getErr() const BY_CONST_FUNC(getErr())
        /**
         * @brief Releases/clears both the contained value and the error, making the instance empty.
         */
        void rel() override;
        /**
         * @brief Sets the contained value and clears any existing error.
         */
        void set(const T& arg) override;
        /**
         * @brief Sets the contained error and clears any existing value.
         */
        void setErr(const R& arg);

    private:
        std::optional<R> _err;
    };
}
