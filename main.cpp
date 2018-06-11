#include <iostream>
#include "lib/core.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "lib/menu.h"

int main(int argc, char* argv[]) {

    menu();

    // for unit testing below
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}

