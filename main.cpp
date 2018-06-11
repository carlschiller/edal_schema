#include <iostream>
#include "lib/core.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char* argv[]) {

    // for unit testing below
    testing::InitGoogleTest(&argc,argv);
    RUN_ALL_TESTS();
    return 0;
}

