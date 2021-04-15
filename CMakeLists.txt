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

# -- fetch actor-framework repo ------------------------------------------------

FetchContent_Declare(
  actor_framework
  GIT_REPOSITORY https://github.com/actor-framework/actor-framework.git
  GIT_TAG        master
)
FetchContent_Populate(actor_framework)
# CAF >= 0.18 setup
foreach(varname CAF_ENABLE_EXAMPLES CAF_ENABLE_IO_MODULE CAF_ENABLE_TESTING
                CAF_ENABLE_TOOLS CAF_ENABLE_OPENSSL_MODULE)
  set(${varname} OFF CACHE INTERNAL "")
endforeach()
set(CAF_SANITIZERS "${SANITIZERS}" CACHE INTERNAL "")
add_subdirectory(${actor_framework_SOURCE_DIR} ${actor_framework_BINARY_DIR})

# -- project files -------------------------------------------------------------

set(SRC_FILES)

# -- add executables -----------------------------------------------------------

add_executable(packet-routing
               "src/main.cpp")

target_include_directories(packet-routing PRIVATE
                           "header")

target_link_libraries(packet-routing PRIVATE 
                      CAF::core)
