cmake_minimum_required(VERSION 3.1.0)

project(kSTL)

file(GLOB SRC_FILES
    "src/*.cpp"
)

file(GLOB HDR_FILES
    "inc/*.hpp"
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
#find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
#find_package(Qt5PrintSupport REQUIRED)
#find_package(Qt5Network REQUIRED)

set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES})

#find_library(wiringPi_LIB wiringPi)
#target_link_libraries(${PROJECT_NAME} ${wiringPi_LIB})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Core)
#target_link_libraries(${PROJECT_NAME} Qt5::Network)
set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")