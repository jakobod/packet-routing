# -- project setup -------------------------------------------------------------

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(packet-routing CXX)

include(FetchContent)

# -- set useful CMake options --------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
add_compile_options(-Wall -Wextra -pedantic)

# -- add external libraries ----------------------------------------------------

find_package(Boost 1.65 COMPONENTS graph REQUIRED)

# -- fetch actor-framework repo ------------------------------------------------

FetchContent_Declare(
  actor_framework
  GIT_REPOSITORY https://github.com/actor-framework/actor-framework.git
  GIT_TAG        master
)
FetchContent_Populate(actor_framework)
foreach(varname CAF_ENABLE_EXAMPLES CAF_ENABLE_IO_MODULE CAF_ENABLE_TESTING
                CAF_ENABLE_TOOLS CAF_ENABLE_OPENSSL_MODULE)
  set(${varname} OFF CACHE INTERNAL "")
endforeach()
set(CAF_SANITIZERS "${SANITIZERS}" CACHE INTERNAL "")
add_subdirectory(${actor_framework_SOURCE_DIR} ${actor_framework_BINARY_DIR})

# -- project files -------------------------------------------------------------

# Find the packet-routing headers
file(GLOB_RECURSE PACKET_ROUTING_HEADERS "header/*.hpp")

# New source files have to be added here
set(
  PACKET_ROUTING_SOURCES
  src/actors/message_generator.cpp
  src/actors/node.cpp
  src/actors/topology_manager.cpp
  src/actors/transition.cpp
  src/graph/edge.cpp
  src/graph/generator.cpp
  src/routing/ant.cpp
  src/routing/message.cpp
  src/routing/entry.cpp
  src/routing/random.cpp
  src/benchmark/benchmarker.cpp
)

set(obj_lib_target "packet_routing_obj")
set(tst_bin_target "packet_routing_test")
add_library(${obj_lib_target} OBJECT
            ${PACKET_ROUTING_HEADERS} ${PACKET_ROUTING_SOURCES})
set_property(TARGET ${obj_lib_target} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(${obj_lib_target} PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}/header")
caf_target_link_libraries(${obj_lib_target}
                          CAF::core Boost::graph)

# -- add executables -----------------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

macro(add_target name)
  add_executable(${name} "src/${name}.cpp")
  target_include_directories(${name} PRIVATE "header")
  target_link_libraries(${name} PRIVATE packet_routing_obj)
endmacro()

add_target(main) # This is the main project file

# -- test setup ----------------------------------------------------------------

enable_testing()

function(add_test_suites target)
  foreach(suiteName ${ARGN})
    string(REPLACE "." "/" suitePath ${suiteName})
    target_sources(${target} PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/test/${suitePath}.cpp")
    add_test(NAME ${suiteName}
             COMMAND ${target} -r300 -n -v5 -s${suiteName})
  endforeach()
endfunction()

add_executable(${tst_bin_target}
               "${CMAKE_CURRENT_SOURCE_DIR}/test/packet_routing_test.cpp"
               $<TARGET_OBJECTS:${obj_lib_target}>)
target_link_libraries(${tst_bin_target} PRIVATE
                      packet_routing_obj CAF::test)
target_include_directories(${tst_bin_target} PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}/header")
add_test_suites(${tst_bin_target}
                actors.message_generator
                actors.node
                actors.topology_manager
                actors.transition
                graph.generator
                routing.ant
                routing.entry
                routing.message
                routing.random)
