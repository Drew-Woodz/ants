cmake_minimum_required(VERSION 3.21)
project(Myrmidon LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Clang warnings as errors from day 1
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Weverything -Werror -Wno-c++98-compat -Wno-padded -Wno-exit-time-destructor)
endif()

add_subdirectory(external/raylib)
add_subdirectory(external/json)

add_executable(Myrmidon
    src/main.cpp
)

target_link_libraries(Myrmidon PRIVATE raylib)

# Copy assets folder at build time – works with Ninja AND Visual Studio generators
add_custom_command(TARGET Myrmidon POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Myrmidon>/assets"
        "${CMAKE_SOURCE_DIR}/config"
        "$<TARGET_FILE_DIR:Myrmidon>/config"
)
