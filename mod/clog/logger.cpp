#include "logger.hpp"
#include <iostream>
#include "stream.hpp"

namespace namu {

    NAMU_DEF_ME(logger)
    typedef std::string string;

    const std::string& me::getName() const {
        static std::string inner("logger");
        return inner;
    }

    stream& me::operator[](nidx n) { return getStream(n); }
    stream& me::operator[](const nchar* msg) { return getStream(msg); }
    stream& me::operator[](const std::string& msg) { return getStream(msg); }

    stream& me::getStream(nidx n) {
        if(n < 0 || n >= getStreamCount())
            return nulOf<stream>();

        return *_streams[n];
    }

    stream& me::getStream(const std::string& msg) {
        for(auto e : _streams)
            if(string(e->getName()) == msg)
                return *e;

        return nulOf<stream>();
    }

    stream& me::getStream(const nchar* msg) {
        return getStream(std::string(msg));
    }

    ncnt me::getStreamCount() const { return _streams.size(); }

    nbool me::logBypass(const nchar* message) {
        if(!isEnable()) return false;

        nbool result = false;
        for(auto e : _streams)
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

    nbool me::pushStream(stream* new_stream) {
        if( ! new_stream) return true;

        _streams.push_back(new_stream);
        if(isInit())
            return new_stream->init();
        return false;
    }

    nbool me::init() {
        rel();
        super::init();

        static stream* streams[] = {new consoleStream(), new fileLogStream("./logs"), 0};
        stream* e = 0;
        for(int n=0; (e = streams[n]) ;n++)
            pushStream(e);

        return false;
    }

    nbool me::isInit() const {
        for(auto e : _streams)
            if( ! e->isInit())
                return false;

        return true;
    }

    nbool me::rel() {
        for(auto e : _streams) {
            e->rel();
            delete e;
        }
        _streams.clear();
        return super::rel();
    }

    me& me::get() {
        static me* inner = 0;
        if(nul(inner)) {
            inner = new me();
            inner->init();
        }

        return *inner;
    }

    nbool me::log(logLv::level lv, const nchar* tag, const nchar* filename, const nchar* func, int line, const nchar* fmt, ...) {
        using platformAPI::foreColor;
        std::string msg;
        msg += foreColor(BROWN);
        msg += _makeStr("%s ", platformAPI::createNowTime("%b %d %Y  %X").c_str());

        consoleColor clrLv = WHITE;
        switch(lv) {
            case logLv::ERR: clrLv = LIGHTRED; break;
            case logLv::WARN: clrLv = YELLOW; break;
            case logLv::INFO: clrLv = LIGHTBLUE; break;
        }
        msg += _makeStr("%s%s %s%s <%s::%s#%d> %s",
            tag, foreColor(clrLv).c_str(), logLv::getName(lv).c_str(), foreColor(GREEN).c_str(),
            filename, func, line, foreColor(LIGHTGRAY).c_str()
        );

        va_list va;
        va_start(va, fmt);
        msg += _makeStr(fmt, va);
        va_end(va);

        if(_filters)
            msg = _filters->filt(lv, tag, msg);

        if(!msg.empty())
            return logBypass(msg.c_str());
        return false;
    }

    me::logger() : super(), _filters(nullptr) {}
    me::logger(const me& rhs) : super(rhs), _streams(rhs._streams), _filters(rhs._filters) {}

    nbool me::isEnable() const {
        for(stream* s : _streams)
            if(s->isEnable()) return true;
        return false;
    }

    void me::setEnable(nbool enable) {
        for(stream* s : _streams)
            s->setEnable(enable);
    }

    enables me::getEnables() const {
        enables ret;
        for(stream* s : _streams)
            ret[s->getName()] = s->isEnable();
        return ret;
    }

    void me::setEnables(const enables& enbs) {
        if(enbs.size() <= 0 || enbs.size() != _streams.size()) return;

        for(auto e : enbs) {
            auto& s = getStream(e.first);
            if(!nul(s))
                s.setEnable(e.second);
        }
    }

    void me::setFilters(const filters& newFilters) {
        _filters = &newFilters;
    }

    void me::setFilters() {
        _filters = nullptr;
    }

    const filters& me::getFilters() const {
        return *_filters;
    }

    std::string me::_makeStr(const nchar* fmt, ...) {
        va_list va;
        va_start(va, fmt);
        std::string ret = _makeStr(fmt, va);
        va_end(va);
        return ret;
    }

    std::string me::_makeStr(const nchar* fmt, va_list va) {
        nchar buf[1024];
        vsnprintf(buf, 1024, fmt, va);
        return std::string(buf);
    }
}
