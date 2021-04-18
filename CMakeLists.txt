# -- project setup -------------------------------------------------------------

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(packet-routing CXX)

include(FetchContent)

# -- set useful CMake options --------------------------------------------------

add_compile_options("-std=c++17")

# Enable ASAN if requested by the user.
if (CAF_ENABLE_ADDRESS_SANITIZER AND NOT WIN32)
  add_compile_options("-fsanitize=address"
                      "-fno-omit-frame-pointer")
  list(APPEND CAF_EXTRA_LDFLAGS "-fsanitize=address")
endif ()

# -- add external libraries ----------------------------------------------------

find_package(Boost 1.71 COMPONENTS graph REQUIRED)

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

# New source files have to be added here
set(SRC_FILES
  src/actors/node.cpp
  src/actors/transition.cpp
  src/actors/message_generator.cpp
  src/actors/topology_manager.cpp
  src/graph/generator.cpp
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
