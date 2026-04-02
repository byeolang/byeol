#include "frontend/cli.hpp"

using namespace by;

int main(int argc, char* argv[]) {
    if(buildFeature::config::isDbg()) platformAPI::unlimitCoreDump();

    flagArgs a;
    for(int n = 1; n < argc; n++)
        a.add(new nStr(argv[n]));

    return cli().setFlag(cli::LOG_ON_EX | cli::DUMP_ON_EX).setTask(a).work().res;
}
