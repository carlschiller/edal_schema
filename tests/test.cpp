//
// Created by Carl Schiller on 2018-06-11.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ClassName.h"
#include "../lib/core.h"

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
/*
// Testing that we retrieve name etc of a person from special Worker class.
TEST_F(ClassDeclaration, Worker_class_retrieval_test){
    std::string name("Carl Schiller");
    Worker carl(name, Genders::MALE, Positions::EXTRA_WORKER, 199705266436);
    EXPECT_EQ(name,carl.get_name());
    EXPECT_EQ(Genders::MALE, carl.get_gender());
    EXPECT_EQ(Positions::EXTRA_WORKER, carl.get_position());
    EXPECT_EQ(199705266436,carl.get_personal_number());
}

// Changing names in Worker class etc.
TEST_F(ClassDeclaration, Worker_class_change_test){
    std::string name("Carl Schiller");
    Worker carl(name, Genders::MALE, Positions::EXTRA_WORKER, 199705266436);
    carl.change_gender(Genders::FEMALE);
    carl.change_name("Carla");
    std::string new_name("Carla");
    carl.change_personal_number(200001011000);
    carl.change_position(Positions::FULL_TIME);
    EXPECT_EQ(new_name,carl.get_name());
    EXPECT_EQ(Genders::FEMALE, carl.get_gender());
    EXPECT_EQ(Positions::FULL_TIME, carl.get_position());
    EXPECT_EQ(200001011000,carl.get_personal_number());
}
*/