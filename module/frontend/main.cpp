#include "frontend/cli.hpp"

using namespace by;

int main(int argc, char* argv[]) {
    if(buildFeature::config::isDbg()) platformAPI::unlimitCoreDump();

    cli ep;
    flagArgs a;
    for(int n = 1; n < argc; n++)
        a.add(new nStr(argv[n]));

    return ep.setTask(a).work().res;
}
