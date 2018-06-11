//
// Created by Carl Schiller on 2018-06-11.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ClassName.h"

using testing::Eq;

namespace {
    class ClassDeclaration : public testing::Test {
        public:
            ClassName obj;
            ClassDeclaration(){
                obj;
            }
    };
}
TEST_F(ClassDeclaration, name_of_test_1){
    ASSERT_EQ("","");
}