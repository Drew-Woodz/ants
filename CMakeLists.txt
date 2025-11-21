cmake_minimum_required(VERSION 3.21)
project(Myrmidon LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Clang warnings (activates when we switch kit)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Weverything -Werror -Wno-c++98-compat -Wno-padded -Wno-exit-time-destructor)
endif()

# External dependencies
add_subdirectory(external/raylib)
add_subdirectory(external/json)

# ================== MAIN EXECUTABLE ==================
add_executable(Myrmidon
    src/main.cpp
    src/PheromoneGrid.cpp
    src/Ant.cpp          # ← ADDED
    src/AntConfig.cpp    # ← ADDED
)

target_include_directories(Myrmidon PRIVATE src)

target_link_libraries(Myrmidon PRIVATE 
    raylib 
    nlohmann_json::nlohmann_json
)

# Post-build asset/config copy (works on Windows, macOS, Linux)
add_custom_command(TARGET Myrmidon POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_SOURCE_DIR}/assets" 
        "$<TARGET_FILE_DIR:Myrmidon>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_SOURCE_DIR}/config"  
        "$<TARGET_FILE_DIR:Myrmidon>/config"
    COMMENT "Copying assets and config to output directory"
)