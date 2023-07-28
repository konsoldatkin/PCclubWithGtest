cmake_minimum_required(VERSION 3.16)

project(test_PCclub VERSION 1.0)

#set(gtest_force_shared_crt on)



#enable_testing()
# link against gtest or gtest_main as needed. Eg
add_executable(${PROJECT_NAME} test_club.cpp)
target_link_libraries(${PROJECT_NAME}
	core
	gtest_main
)

add_test(NAME test_club COMMAND ${PROJECT_NAME})

add_custom_command(
     TARGET ${PROJECT_NAME}  
     COMMENT "Run tests"
     POST_BUILD 
     COMMAND ${PROJECT_NAME}
)
