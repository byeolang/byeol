#pragma once

#include "../../ast/node.hpp"
#include "../errReport.hpp"
#include "loweventer.hpp"

namespace wrd {

    /// parser is a class that finally parses the wrd file and creates an ast structure.
    /// this has a parser and a scanner created from a parser generator such as bison
    /// internally, and these are called low-level parsers and low-level scanners.
    /// low-level parser/scanner focuses on whether the string given as input matches a
    /// specific grammar, and passes it to the parser when matching.
    /// this 'parser' class creates and returns an appropriate AST structure if it is
    /// matched.
    ///
    /// the reason for constructing a structure like this is to make it easy to replace other
    /// parser generators.
    /// to accomplish it, please note that this parser class should not be dependent to concrete
    /// implementation which was defined at lowparser/scanner.
    ///
    /// lowparser/scanner should be dependent to THIS.
    class parser : public typeProvidable {
        WRD(INTERFACE(parser, typeProvidable))

	public:
		parser() { me::rel(); }

    public:
		me& setReport(errReport& report) {
            _eventer.getReport().bind(report);
			return *this;
		}
        me& setTray(narr& tray) {
            _eventer.setTray(tray);
            return *this;
        }

        /// @param script is null terminated cstring.
        tstr<narr> parse(const wchar* script);

		virtual void rel();

    private:
        loweventer _eventer;
    };
}
