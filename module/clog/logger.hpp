/// @file
#pragma once

#include "clog/enablesZone.hpp"
#include "clog/filter/filters.hpp"
#include "clog/stream/stream.hpp"

namespace by {

    /** @ingroup clog
     *  @brief Main logging interface for the byeol language system
     *  @details A lightweight C++ logging framework facade that enables systematic logging
     *  through multiple logging destinations called @ref stream. Typically used through
     *  accompanying macros.
     *
     *  @section usage Usage
     *  Basic usage with macros:
     *  @code
     *      BY_I("packs[%s] origins loaded.", getName());
     *  @endcode
     *
     *  This produces output like:
     *  ```
     *  Oct 22 2025  21:26:13 I cppPackLo <_loadLibs#49> pack[cpp] origins loaded.
     *  ```
     *
     *  The log shows:
     *  1. Date (Oct 22 2025) and time (21:26:13)
     *  2. Log level (I for Info) - ERR, WARN, INFO are available
     *  3. Class name (cppPackLoading, abbreviated)
     *  4. Function (_loadLibs) and line number (49)
     *  5. Log message (pack[cpp] origins loaded)
     *
     *  @section stream_concept stream
     *  The core of the clog module is @ref stream. A stream represents the logging output
     *  destination. Currently @ref consoleStream and @ref fileLogStream exist. All streams
     *  are owned by the logger class from the start.
     *
     *  Each stream follows the standard byeol state diagram:
     *  ```
     *  RELEASED ---init()---> INITIALIZED
     *  RELEASED <--rel()----- INITIALIZED
     *  ```
     *
     *  Streams also manage enable status. Using setEnable(false) disables a specific stream.
     *  The logger class itself inherits from stream, so it provides the same API. Logger
     *  redirects each API call to all owned streams. For example, `logger::get().setEnable(false)`
     *  disables all streams.
     *
     *  @section logging_macros macro
     *  Use logging macros for advanced features like filename, function name, and line numbers:
     *  @code
     *      BY_I("just message.");
     *      BY_DE("leaf: ERR: %s", e); // Debug-only error log with format specifiers
     *  @endcode
     *
     *  Macros follow byeol convention with `BY_` prefix, followed by log level (E/W/I).
     *  Add `D` prefix for debug-only logs (e.g., BY_DE, BY_DW, BY_DI).
     *
     *  @section rich_log richLog
     *  The @ref richLog feature uses polymorphism to convert format specifier arguments to
     *  appropriate types:
     *  @code
     *      BY_I("make a closure for %s.%s", meObj, cast.getSrc().getName());
     *  @endcode
     *
     *  Output: `Oct 22 2025 22:01:12 I closure <_make#73> make a closure for obj.foo`
     *
     *  Both tstr<obj> and std::string are properly converted to strings. Since clog is a
     *  low-level module in the architecture, richLog is defined per-module to handle
     *  module-specific class conversions.
     *
     *  @section filtering filterable
     *  Log messages can be filtered based on specific conditions. The @ref filterable
     *  interface's `filt()` function determines filtering. Add filters using
     *  `setFilters(const filters&)`. For example, @ref errPassFilter only passes ERR level
     *  messages.
     */
    class _nout logger: public stream {
        BY_ME(logger, stream)

    public:
        stream* getStream(nidx n);
        const stream* getStream(nidx n) const BY_CONST_FUNC(getStream(n))
        stream* getStream(const nchar* msg);
        const stream* getStream(const nchar* msg) const BY_CONST_FUNC(getStream(msg))
        stream* getStream(const std::string& msg);
        const stream* getStream(const std::string& msg) const BY_CONST_FUNC(getStream(msg))

        nbool isEnable() const override;
        void setEnable(nbool enable) override;
        enables getEnables() const;
        void setEnables(const enables& enbs);
        ncnt getStreamCount() const;

        /**
         * @brief Logs formatted message bypassing rich logging conversion
         * @return true on success, false on error
         */
        nbool logFormatBypass(const nchar* fmt, ...);

        /**
         * @brief Main logging function with full context information
         * @return true on success, false on error
         */
        nbool log(errLv::level lv, const std::string& filename, const nchar* func, int line, const nchar* fmt, ...);

        /**
         * @brief Adds a new logging stream to the logger
         * @return true on success, false on error
         */
        /**
         * @brief Adds a new logging stream to the logger.
         * @param new_stream Pointer to the stream to add.
         * @return true on success, false on error (e.g., stream already added or invalid).
         */
        nbool pushStream(stream* new_stream);
        /**
         * @brief Adds a new logging stream to the logger.
         * @param new_stream Reference to the stream to add.
         * @return true on success, false on error (e.g., stream already added or invalid).
         */
        nbool pushStream(stream& new_stream);

        /**
         * @brief Get singleton logger instance
         */
        static logger& get();

        //  stream:
        const std::string& getName() const override;

        using super::logBypass;

        /**
         * @brief Log message bypassing normal filtering
         * @return true on success, false on error
         */
        nbool logBypass(const nchar* message) override;

        nbool isInit() const override;

        /**
         * @brief Set logging filters
         */
        void setFilters(const filters& newFilters);

        /**
         * @brief Reset filters to default
         */
        void setFilters();

        const filters& getFilters() const;

    private:
        //  Logger:
        logger();
        logger(const logger& rhs);
        //  stream:
        nbool init() override;
        nbool rel() override;

        std::string _makeStr(const nchar* fmt, ...);
        std::string _makeStr(const nchar* fmt, va_list va);

    private:
        std::vector<stream*> _streams;
        const filters* _filters;
    };

    logger& operator<<(logger& log, const std::string& msg);
} // namespace by
