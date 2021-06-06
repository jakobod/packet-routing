# -- project setup -------------------------------------------------------------

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(packet-routing CXX)

include(FetchContent)

# -- set useful CMake options --------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
add_compile_options(-Wall -Wextra -pedantic)

# Enable ASAN if requested by the user.
if (CAF_ENABLE_ADDRESS_SANITIZER AND NOT WIN32)
  add_compile_options("-fsanitize=address"
                      "-fno-omit-frame-pointer")
  list(APPEND CAF_EXTRA_LDFLAGS "-fsanitize=address")
endif ()

# -- add external libraries ----------------------------------------------------

find_package(Boost 1.65 COMPONENTS graph REQUIRED)

# -- fetch actor-framework repo ------------------------------------------------

FetchContent_Declare(
  actor_framework
  GIT_REPOSITORY https://github.com/actor-framework/actor-framework.git
  GIT_TAG        master
)
FetchContent_Populate(actor_framework)
foreach(varname CAF_ENABLE_EXAMPLES CAF_ENABLE_IO_MODULE
                CAF_ENABLE_TOOLS CAF_ENABLE_OPENSSL_MODULE)
  set(${varname} OFF CACHE INTERNAL "")
endforeach()
set(CAF_SANITIZERS "${SANITIZERS}" CACHE INTERNAL "")
add_subdirectory(${actor_framework_SOURCE_DIR} ${actor_framework_BINARY_DIR})

# -- project files -------------------------------------------------------------

# New source files have to be added here
set(
  SRC_FILES
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

# -- add executables -----------------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

macro(add_target name)
  add_executable(${name}
                 "src/${name}.cpp"
                 "${SRC_FILES}")
  target_include_directories(${name} PRIVATE
                             "header")
  target_link_libraries(${name} PRIVATE 
                        CAF::core Boost::graph)
endmacro()

add_target(example) # This is the example project
add_target(main) # This is the main project file

# -- test setup ----------------------------------------------------------------

enable_testing()
function(add_test_suites target)
  foreach(suiteName ${ARGN})
    string(REPLACE "." "/" suitePath ${suiteName})
    target_sources(${target} PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/test/${suitePath}.cpp")
    add_test(NAME ${suiteName}
             COMMAND ${target} -r300 -n -v5 -s"^${suiteName}$")
  endforeach()
endfunction()

add_executable(packet-routing-test 
  "test/packet_routing_test.cpp"  
  ${SRC_FILES})  
target_link_libraries(packet-routing-test PRIVATE
                      CAF::test
                      CAF::core
                      Boost::graph)
target_include_directories(packet-routing-test PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}/test"
                           "${CMAKE_CURRENT_SOURCE_DIR}/header")

set(TEST_SUITES
  transition
  graph
  message_generator
)

add_test_suites(packet-routing-test ${TEST_SUITES})
