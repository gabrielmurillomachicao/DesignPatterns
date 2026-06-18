cmake_minimum_required(VERSION 3.22)
project(EC_patrones_de_diseno LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(app_gastos main.cpp)
target_include_directories(app_gastos PRIVATE include)

add_executable(expenses_tests Tests/test_expenses.cpp)
target_include_directories(expenses_tests PRIVATE include)

enable_testing()
add_test(NAME expenses_tests COMMAND expenses_tests)