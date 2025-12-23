cmake_minimum_required(VERSION 3.14)
project(FlowAnalysis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use FetchContent to get nlohmann/json
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

add_executable(FlowAnalysis 
    src/main.cpp 
    src/NoC.cpp
)

target_link_libraries(FlowAnalysis PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(FlowAnalysis PRIVATE src)
