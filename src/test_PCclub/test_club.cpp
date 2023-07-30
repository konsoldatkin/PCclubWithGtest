
#include <gtest/gtest.h>
#include <stdexcept>
#include "../core/club.hpp" 

#include <exception>
#include <fstream>


using namespace PC_club::core;


TEST(PCclub_test, ExceptionHandling) {

    std::string text_table("../../../../test_data/text_table.txt");

    EXPECT_THROW(PCclub{text_table }, std::invalid_argument);
}

TEST(PCclub_test4, ExceptionHandlin4) {

    std::string emptyString = "";

    EXPECT_THROW(Event{ emptyString }, std::invalid_argument);
}

