#include "cli.hpp"
using namespace wrd;

namespace wrd {

    const flags& cli::getFlags() const {
        static flags inner;
        if(inner.size() <= 0) {
            inner.push_back(tstr<flag>(new helpFlag()));
            inner.push_back(tstr<flag>(new fileFlag()));
            inner.push_back(tstr<flag>(new logStructureFlag()));
            inner.push_back(tstr<flag>(new logInterpreterFlag()));
        }

        return inner;
    }
}

int main(int argc, char* argv[]) {
    // disable console log:
    logger::get()["consoleStream"].setEnable(false);

    cli ep;

    args a;
    for(int n=1; n < argc ;n++)
        a.push_back(argv[n]);

    return ep.run(a);
}
