cmake_minimum_required(VERSION 3.16)cmake_minimum_required(VERSION 3.16)

project(StrengthReducePass LANGUAGES CXX)project(StrengthReducePass LANGUAGES CXX)



find_package(LLVM REQUIRED CONFIG)set(CMAKE_CXX_STANDARD 17)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${LLVM_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

add_library(StrengthReducePass SHARED

    src/StrengthReduce.cppinclude_directories(${LLVM_INCLUDE_DIRS})

)add_definitions(${LLVM_DEFINITIONS})



set_target_properties(StrengthReducePass PROPERTIESadd_library(StrengthReducePass SHARED

    COMPILE_FLAGS "-fno-rtti"    src/StrengthReduce.cpp

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
