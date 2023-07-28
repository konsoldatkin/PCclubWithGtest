#include "core/club.hpp"

#include <filesystem>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Invalid number of params" << argc << std::endl;
        return EXIT_FAILURE;
    }
    std::string file_name = argv[1];
    if (!std::filesystem::exists(file_name)) {
        std::cout << "File does not exist" << std::endl;
        return EXIT_FAILURE;
    }
    auto club = PC_club::core::PCclub(file_name);
    return EXIT_SUCCESS;
}
