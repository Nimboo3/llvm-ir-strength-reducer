cmake_minimum_required(VERSION 3.16)cmake_minimum_required(VERSION 3.16)cmake_minimum_required(VERSION 3.16)

project(StrengthReducePass LANGUAGES CXX)

project(StrengthReducePass LANGUAGES CXX)project(StrengthReducePass LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LLVM REQUIRED CONFIG)set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")set(CMAKE_CXX_STANDARD_REQUIRED ON)



include_directories(${LLVM_INCLUDE_DIRS})set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(${LLVM_DEFINITIONS})

include_directories(${LLVM_INCLUDE_DIRS})

add_library(StrengthReducePass SHARED

    src/StrengthReduce.cppadd_definitions(${LLVM_DEFINITIONS})find_package(LLVM REQUIRED CONFIG)

)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

set_target_properties(StrengthReducePass PROPERTIES

    COMPILE_FLAGS "-fno-rtti"add_library(StrengthReducePass SHARED

)

    src/StrengthReduce.cppinclude_directories(${LLVM_INCLUDE_DIRS})

target_compile_definitions(StrengthReducePass PRIVATE LLVM_ENABLE_STATS)

target_include_directories(StrengthReducePass PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include))add_definitions(${LLVM_DEFINITIONS})



llvm_map_components_to_libnames(REQ_LLVM_LIBS

    Core

    Supportset_target_properties(StrengthReducePass PROPERTIESadd_library(StrengthReducePass SHARED

)

    COMPILE_FLAGS "-fno-rtti"    src/StrengthReduce.cpp

target_link_libraries(StrengthReducePass PRIVATE ${REQ_LLVM_LIBS})

))



llvm_map_components_to_libnames(REQ_LLVM_LIBSset_target_properties(StrengthReducePass PROPERTIES

    Core    COMPILE_FLAGS "-fno-rtti"

    Support)

)

target_compile_definitions(StrengthReducePass PRIVATE LLVM_ENABLE_STATS)

target_link_libraries(StrengthReducePass PRIVATE ${REQ_LLVM_LIBS})target_include_directories(StrengthReducePass PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


llvm_map_components_to_libnames(REQ_LLVM_LIBS
    Core
    Support
)

target_link_libraries(StrengthReducePass PRIVATE ${REQ_LLVM_LIBS})
