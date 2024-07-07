#include "logger.hpp"
#include <iostream>
#include "stream.hpp"
#include <sstream>
#include <iomanip>

namespace nm {

    NM_DEF_ME(logger)
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
    nbool me::logBypass(const std::string& msg) {
        return logBypass(msg.c_str());
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

    namespace {
        std::string _extractTag(const std::string& filename) {
            static std::vector<std::string> exts;
            if(exts.size() <= 0) {
                exts.push_back(".cc");
                exts.push_back(".inl");
                exts.push_back(".cpp");
                exts.push_back(".hpp");
            }

            for(auto ext : exts) {
                nint newN = filename.find(ext.c_str());
                if(newN != string::npos)
                    return filename.substr(0, newN);
            }

            return filename;
        }

        std::string _makeTag(const std::string& filename) {
            // limit tag:
            std::string ret = _extractTag(filename);

            constexpr ncnt MAX = 9;
            if(ret.length() > MAX)
                ret = ret.substr(0, MAX);

            std::stringstream ss;
            ss << std::setw(MAX) << std::left << ret;
            return ss.str();
        }
    }

    nbool me::log(logLv::level lv, const std::string& filename, const nchar* func, int line, const nchar* fmt, ...) {
        using platformAPI::foreColor;
        std::string msg;
        msg += foreColor(BROWN);
        msg += _makeStr("%s ", platformAPI::createNowTime("%b %d %Y  %X").c_str());
        std::string tag = _makeTag(filename);

        consoleColor clrLv = WHITE;
        switch(lv) {
            case logLv::ERR: clrLv = LIGHTRED; break;
            case logLv::WARN: clrLv = YELLOW; break;
            case logLv::INFO: clrLv = LIGHTBLUE; break;
        }
        msg += _makeStr("%s%s %s%s <%s%s#%d> %s",
            foreColor(clrLv).c_str(), logLv::getName(lv).c_str(),
            foreColor(LIGHTMAGENTA).c_str(), tag.c_str(),
            foreColor(GREEN).c_str(), func, line, foreColor(LIGHTGRAY).c_str()
        );

        va_list va;
        va_start(va, fmt);
        msg += _makeStr(fmt, va);
        va_end(va);

        if(_filters)
            msg = _filters->filt(lv, tag.c_str(), msg);

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

    std::string convert(const nint& it) { return std::to_string(it); }
    std::string convert(const nflt& it) { return std::to_string(it); }
    std::string convert(const nchar& it) { return std::to_string(it); }
    std::string convert(const nbool& it) { return it ? "true" : "false"; }
    std::string convert(const std::string& it) { return it; }
    std::string convert(const nchar* it) { return std::string(it); }
    std::string convert(const ndbl& it) { return std::to_string(it); }
    std::string convert(const nuint& it) { return std::to_string(it); }
    std::string convert(const nlong& it) { return std::to_string(it); }
    std::string convert(const nulong& it) { return std::to_string(it); }
    std::string convert(void* it) { return it ? platformAPI::toAddrId(it) : "null"; }
}
