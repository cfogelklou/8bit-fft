cmake_minimum_required(VERSION 3.5)
project(8bit_fft)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DSODIUM_STATIC -DDEBUG -D_CONSOLE -DNATIVE_LITTLE_ENDIAN)

set(TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB LOCAL_SRC
        ${TOP_DIR}/src/*.c
        ${TOP_DIR}/src/*.cpp
        ${TOP_DIR}/src/*.h
        ${TOP_DIR}/src/*.hpp
        ${TOP_DIR}/src/*.s
        ${TOP_DIR}/test/*.c
        ${TOP_DIR}/test/*.cpp
        ${TOP_DIR}/test/*.h
        ${TOP_DIR}/test/*.hpp
)

set(GTEST_SRC
        ${TOP_DIR}/googletest/googletest/src/gtest-all.cc
        ${TOP_DIR}/googletest/googlemock/src/gmock-all.cc
)

set(SOURCE_FILES
        ${LOCAL_SRC}

        ${GTEST_SRC}
)

include_directories(
       .
       # Google test (the real thing)
       ${TOP_DIR}/googletest/googletest
       ${TOP_DIR}/googletest/googletest/include
       ${TOP_DIR}/googletest/googlemock
       ${TOP_DIR}/googletest/googlemock/include
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# target_link_libraries(${PROJECT_NAME} curl)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
else (WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} Threads::Threads)
endif (WIN32)

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})
