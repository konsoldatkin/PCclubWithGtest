
#include <gtest/gtest.h>
#include <stdexcept>
#include "../core/club.hpp" 

#include <exception>
#include <fstream>


using namespace PC_club::core;


TEST(PCclub_test, ExceptionHandling) {

    std::string text_table("../../../../test_data/text_table.txt");

    EXPECT_THROW(PCclub{ text_table };, std::logic_error);

    EXPECT_THROW(PCclub{ text_table };, std::invalid_argument);
}
    
TEST(PCclub_test1, ExceptionHandling1) {

    std::string text_open_close = "../../../../test_data/text_open_close_time.txt";

    EXPECT_THROW(PCclub{ text_open_close }; , std::logic_error);

    EXPECT_THROW(PCclub{ text_open_close }; , std::invalid_argument);
}

TEST(PCclub_test2, ExceptionHandling2) {

    std::string text_price ="../../../../test_data/text_price.txt";

    EXPECT_THROW(PCclub{ text_price }; , std::logic_error);

    EXPECT_THROW(PCclub{ text_price }; , std::invalid_argument);
}

TEST(PCclub_test3, ExceptionHandling3) {

    std::string text_name = "../../../../test_data/text_name.txt";

    EXPECT_THROW(PCclub{ text_name }; , std::logic_error);

    EXPECT_THROW(PCclub{ text_name }; , std::invalid_argument);
}

TEST(PCclub_test4, ExceptionHandlin4) {

    std::string text_time = "../../../../test_data/text_time.txt";

    EXPECT_THROW(PCclub{ text_time }; , std::logic_error);

    EXPECT_THROW(PCclub{ text_time }; , std::invalid_argument);
}

