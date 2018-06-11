#include <iostream>
#include "core.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char* argv[]) {
    std::string name("Carl Schiller");
    Worker carl(name, Genders::SEX_TYPE_APPACHE_HELICOPTER, Positions::EXTRA_WORKER, 199705266436);
    std::cout << carl.get_name() << std::endl;

    testing::InitGoogleTest(&argc,argv);
    RUN_ALL_TESTS();
    return 0;
}

