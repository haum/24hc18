CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(MarabuntaEngineTest)

SET(GTestSrc /usr/src/gtest)
SET(GMockSrc /usr/src/gmock)
SET(GTestInc /usr/include/gtest)
SET(GMockInc /usr/include/gmock)
add_definitions(-DGTEST_LANGUAGE_CXX11)
include_directories(${GTestSrc} ${GTestInc} ${GMockSrc} ${GMockInc})

add_test(${PROJECT_NAME} COMMAND ${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp
               ${GTestSrc}/src/gtest-all.cc
               ${GMockSrc}/gmock-all.cc

               test_gears_lineparser.cpp

               ../engine/gears/lineparser.cpp
               )

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
