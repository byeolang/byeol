/// @file
#pragma once

#include "clog/enablesZone.hpp"
#include "clog/filter/filters.hpp"
#include "clog/stream/stream.hpp"

namespace by {

    /// @ingroup clog
    /// @brief Main logging interface for the byeol language system
    /// @details Manages multiple output streams and filtering capabilities.
    /// Provides centralized logging with configurable output destinations
    /// and severity level filtering.
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

        /// @brief Enable or disable all logging streams
        /// @param enable True to enable all streams, false to disable
        void setEnable(nbool enable) override;
        enables getEnables() const;
        void setEnables(const enables& enbs);
        ncnt getStreamCount() const;
        nbool logFormatBypass(const nchar* fmt, ...);
        nbool log(errLv::level lv, const std::string& filename, const nchar* func, int line, const nchar* fmt, ...);

        nbool pushStream(stream* new_stream);
        nbool pushStream(stream& new_stream);

        /// @brief Get singleton logger instance
        static logger& get();

        //  stream:
        const std::string& getName() const override;

        using super::logBypass;
        /// @brief Log message bypassing normal filtering
        /// @param message Message to log directly
        nbool logBypass(const nchar* message) override;

        nbool isInit() const override;
        //  filter:
        /// @brief Set logging filters
        /// @param newFilters Filter configuration to apply
        void setFilters(const filters& newFilters);
        /// @brief Reset filters to default
        void setFilters();
        /// @brief Get current filter configuration
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
