#include "clog/logger.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "clog/stream.hpp"

namespace by {


    BY_DEF_ME(logger)
    typedef std::string string;

    me& operator<<(me& log, const std::string& msg) {
        log.logBypass(msg);
        return log;
    }

    const std::string& me::getName() const {
        static std::string inner("logger");
        return inner;
    }

    stream* me::getStream(nidx n) { return _streams[n]; }

    stream* me::getStream(const std::string& msg) {
        for(auto e: _streams)
            if(string(e->getName()) == msg) return e;

        return nullptr;
    }

    stream* me::getStream(const nchar* msg) { return getStream(std::string(msg)); }

    ncnt me::getStreamCount() const { return _streams.size(); }

    nbool me::logBypass(const nchar* message) {
        if(!isEnable()) return false;

        nbool result = false;
        for(auto e: _streams)
            result |= e->logBypass(message);

        return result;
    }

    nbool me::logFormatBypass(const nchar* fmt, ...) {
        va_list va;
        va_start(va, fmt);
        nbool ret = logBypass(_makeStr(fmt, va).c_str());
        va_end(va);
        return ret;
    }

    nbool me::pushStream(stream* newStream) {
        WHEN_NUL(newStream).ret(true);
        return pushStream(*newStream);
    }

    nbool me::pushStream(stream& newStream) {
        _streams.push_back(&newStream);
        if(isInit()) return newStream.init();
        return false;
    }

    nbool me::init() {
        rel();
        super::init();

        static stream* streams[] = {new consoleStream(), new fileLogStream("./logs"), 0};
        stream* e = 0;
        for(int n = 0; (e = streams[n]); n++)
            pushStream(e);

        return false;
    }

    nbool me::isInit() const {
        for(auto e: _streams)
            if(!e->isInit()) return false;

        return true;
    }

    nbool me::rel() {
        for(auto e: _streams) {
            e->rel();
            delete e;
        }
        _streams.clear();
        return super::rel();
    }

    me& me::get() {
        static me* inner = 0;
        if(!inner) {
            inner = new me();
            inner->init();
        }

        return *inner;
    }

    namespace {
        std::string _extractTag(const std::string& filename) {
            static std::vector<std::string> exts;
            if(exts.size() <= 0) {
                exts.push_back(".cc");
                exts.push_back(".inl");
                exts.push_back(".cpp");
                exts.push_back(".hpp");
            }

            for(const auto& ext: exts) {
                auto newN = filename.find(ext.c_str());
                if(newN != string::npos) return filename.substr(0, newN);
            }

            return filename;
        }

        std::string _makeTag(const std::string& filename) {
            // limit tag:
            std::string ret = _extractTag(filename);

            constexpr ncnt MAX = 9;
            if(ret.length() > MAX) ret = ret.substr(0, MAX);

            std::stringstream ss;
            ss << std::setw(MAX) << std::left << ret;
            return ss.str();
        }
    }

    nbool me::log(errLv::level lv, const std::string& filename, const nchar* func, int line, const nchar* fmt, ...) {
        std::string tag = _makeTag(filename);
        WHEN(_filters && !_filters->filt(lv, tag.c_str())) .ret(false);

        using platformAPI::foreColor;
        *this << foreColor(BROWN) << _makeStr("%s ", platformAPI::createNowTime("%b %d %Y  %X").c_str());

        consoleColor clrLv = WHITE;
        switch(lv) {
            case errLv::ERR: clrLv = LIGHTRED; break;
            case errLv::WARN: clrLv = YELLOW; break;
            case errLv::INFO: clrLv = LIGHTBLUE; break;
        }
        *this << foreColor(clrLv) << std::string(1, errLv::getName(lv)[0]) << " " << foreColor(LIGHTMAGENTA) << tag
              << foreColor(GREEN) << " <" << func << "#" << std::to_string(line) << "> " << foreColor(LIGHTGRAY);

        va_list va;
        va_start(va, fmt);
        *this << _makeStr(fmt, va);
        va_end(va);
        return false;
    }

    me::logger(): super(), _filters(nullptr) {}

    me::logger(const me& rhs): super(rhs), _streams(rhs._streams), _filters(rhs._filters) {}

    nbool me::isEnable() const {
        for(stream* s: _streams)
            if(s->isEnable()) return true;
        return false;
    }

    void me::setEnable(nbool enable) {
        for(stream* s: _streams)
            s->setEnable(enable);
    }

    enables me::getEnables() const {
        enables ret;
        for(stream* s: _streams)
            ret[s->getName()] = s->isEnable();
        return ret;
    }

    void me::setEnables(const enables& enbs) {
        if(enbs.size() <= 0 || enbs.size() != _streams.size()) return;

        for(const auto& e: enbs) {
            auto* s = getStream(e.first);
            if(s) s->setEnable(e.second);
        }
    }

    void me::setFilters(const filters& newFilters) { _filters = &newFilters; }

    void me::setFilters() { _filters = nullptr; }

    const filters& me::getFilters() const { return *_filters; }

    std::string me::_makeStr(const nchar* fmt, ...) {
        va_list va;
        va_start(va, fmt);
        std::string ret = _makeStr(fmt, va);
        va_end(va);
        return ret;
    }

    namespace {
        constexpr ncnt BUF_LEN = 1024;
    }

    std::string me::_makeStr(const nchar* fmt, va_list va) {
        nchar buf[BUF_LEN];
        vsnprintf(buf, BUF_LEN, fmt, va);
        return std::string(buf);
    }
} // namespace by
