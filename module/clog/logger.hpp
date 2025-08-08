#pragma once

#include "clog/enablesZone.hpp"
#include "clog/filter/filters.hpp"
#include "clog/stream/stream.hpp"

namespace by {

    class _nout logger: public stream {
        NM_ME(logger, stream)

    public:
        stream* getStream(nidx n);
        const stream* getStream(nidx n) const NM_CONST_FUNC(getStream(n))
        stream* getStream(const nchar* msg);
        const stream* getStream(const nchar* msg) const NM_CONST_FUNC(getStream(msg))
        stream* getStream(const std::string& msg);
        const stream* getStream(const std::string& msg) const NM_CONST_FUNC(getStream(msg))

        nbool isEnable() const override;

        /// modify that all of streams are set to enable or not.
        void setEnable(nbool enable) override;
        enables getEnables() const;
        void setEnables(const enables& enbs);
        ncnt getStreamCount() const;
        nbool logFormatBypass(const nchar* fmt, ...);
        nbool log(errLv::level lv, const std::string& filename, const nchar* func, int line,
            const nchar* fmt, ...);

        nbool pushStream(stream* new_stream);
        nbool pushStream(stream& new_stream);

        static logger& get();

        //  stream:
        const std::string& getName() const override;

        using super::logBypass;
        nbool logBypass(const nchar* message) override;

        nbool isInit() const override;
        //  filter:
        void setFilters(const filters& newFilters);
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
