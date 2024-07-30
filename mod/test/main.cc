#include "common/dep.hpp"

static bool _isVerbose(int argc, char** argv) {
    for(int n = 1; n < argc; n++)
        if(argv[n] == std::string("verbose")) return true;
    return false;
}

int main(int argc, char** argv) {
    nm::logger::get().setEnable(_isVerbose(argc, argv));

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
