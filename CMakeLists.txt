cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(mlFORoa)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package(ROOT REQUIRED COMPONENTS RooStats)

include(${ROOT_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

file(GLOB DAT_FILES
    "*.dat")

file(COPY ${DAT_FILES} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB CFG_FILES
    "*.cfg")

file(COPY ${CFG_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(app app.cc ${sources} ${headers})
target_link_libraries(app ${ROOT_LIBRARIES})

add_custom_target(mlFORoa DEPENDS app)

