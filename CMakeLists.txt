# -- project setup -------------------------------------------------------------

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(packet-routing CXX)

include(FetchContent)

# -- set useful CMake options --------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")

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
foreach(varname CAF_ENABLE_EXAMPLES CAF_ENABLE_IO_MODULE CAF_ENABLE_TESTING
                CAF_ENABLE_TOOLS CAF_ENABLE_OPENSSL_MODULE)
  set(${varname} OFF CACHE INTERNAL "")
endforeach()
set(CAF_SANITIZERS "${SANITIZERS}" CACHE INTERNAL "")
add_subdirectory(${actor_framework_SOURCE_DIR} ${actor_framework_BINARY_DIR})

# -- fetch actor-framework repo ------------------------------------------------

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# -- project files -------------------------------------------------------------

# New source files have to be added here
set(SRC_FILES
  src/actors/message_generator.cpp
  src/actors/node.cpp
  src/actors/topology_manager.cpp
  src/actors/transition.cpp
  src/graph/edge.cpp
  src/graph/generator.cpp
  src/routing/message.cpp
  src/routing/table.cpp
  src/routing/entry.cpp
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

# -- add test executables -------------------------------------------------------

enable_testing()

add_executable(
  vas_test
  "${SRC_FILES}"
  test/generator.cpp
)

target_include_directories(vas_test PRIVATE "header")

target_link_libraries(
  vas_test
  gtest_main
  CAF::core
  Boost::graph
)

include(GoogleTest)
gtest_discover_tests(vas_test)
