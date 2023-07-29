cmake_minimum_required(VERSION 3.4...3.18)
project(pybind_experiments)


set(CMAKE_PREFIX_PATH ${TORCH_CMAKE_PREFIX_PATH})


find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_subdirectory(pybind11)
pybind11_add_module(simple_functions src/simple_functions.cpp)
pybind11_add_module(astar src/astar.cpp)

pybind11_add_module(torch_functions src/torch_functions.cpp)
target_link_libraries(torch_functions PRIVATE ${TORCH_LIBRARIES})
target_include_directories(torch_functions PRIVATE ${TORCH_INCLUDE_DIRS})
target_compile_features(torch_functions PUBLIC cxx_std_17)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(simple_functions
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
target_compile_definitions(astar
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
target_compile_definitions(torch_functions
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
