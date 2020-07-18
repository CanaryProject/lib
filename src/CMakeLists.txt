cmake_minimum_required(VERSION 3.10 FATAL_ERROR)


# *****************************************************************************
# Cmake Features
# *****************************************************************************
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Global gives error when building spdlog
# Avoid changes by cmake/make on the source tree
# https://gitlab.kitware.com/cmake/cmake/issues/18403
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Make will print more details
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# *****************************************************************************
# Options
# *****************************************************************************
option(OPTIONS_ENABLE_CTAGS "Enable automatic tags generation with ctags" OFF)
option(OPTIONS_ENABLE_DEBUG_LOG "Enable debug log" OFF)
option(OPTIONS_ENABLE_OPENMP "Enable Open Multi-Processing support." ON)
option(OPTIONS_ENABLE_UNIT_TEST "Enable Unit-Test Build" OFF)
option(OPTIONS_WARNINGS_FLAGS "Enable the warning flags" ON)
option(OPTIONS_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)


# *****************************************************************************
# CanaryLib
# *****************************************************************************
add_library(canary_lib STATIC "")


# *****************************************************************************
# Define Default Build Type
# *****************************************************************************
# -DCMAKE_BUILD_TYPE=<Release|Debug|RelWithDebInfo>
set(default_build_type "RelWithDebInfo")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  log_info("Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


# *****************************************************************************
# Options Code
# *****************************************************************************

# === CTAGS ===
if(OPTIONS_ENABLE_CTAGS)
  log_option_enabled("tags")
  find_program(CTAGS ctags)
  if(CTAGS)
      add_custom_target(tags_generation ALL
                        COMMAND ${CTAGS} -R --fields=+iaS --extra=+q --language-force=C++ ${CMAKE_CURRENT_SOURCE_DIR}
                        COMMENT "Generating tags file..."
                        )
  else()
      log_program_missing("ctags")
  endif()
else()
    log_option_disabled("tags")
endif()


# === Debug Log ===
# -DOPTIONS_ENABLE_DEBUG_LOG=ON
if(OPTIONS_ENABLE_DEBUG_LOG)
  add_definitions( -DDEBUG_LOG )
  log_option_enabled("debug log")
else()
  log_option_disabled("debug log")
endif()


# === OpenMP ===
if(OPTIONS_ENABLE_OPENMP)
  log_option_enabled("openmp")
  find_package(OpenMP)
  if(OpenMP_CXX_FOUND)
      target_link_libraries(canary_lib PUBLIC OpenMP::OpenMP_CXX)
  endif()
else()
  log_option_disabled("openmp")
endif()


# === Unit Test ===
# -DOPTIONS_ENABLE_UNIT_TEST=ON
if(OPTIONS_ENABLE_UNIT_TEST)
  log_option_enabled("unit-test")
  add_subdirectory(../tests tests)
else()
  log_option_disabled("unit-test")
endif()


# === Add Warnings Flags ===
if(OPTIONS_WARNINGS_FLAGS)
  log_option_enabled("warning flags")
  set(CLANG_WARNINGS -Wall
                    -Wextra
                    -Wpedantic
                    -Wshadow
                    -Wnon-virtual-dtor
                    -Wold-style-cast
                    -Wcast-align
                    -Wunused
                    -Woverloaded-virtual
                    -Wsign-conversion
                    -Wnull-dereference
                    -Wdouble-promotion
                    -Wformat=2
                    -Wno-c99-extensions
    )

  set(GCC_WARNINGS ${CLANG_WARNINGS}
                  -Wmisleading-indentation
                  -Wduplicated-cond
                  -Wduplicated-branches
                  -Wlogical-op
                  -Wuseless-cast
    )

  if(MSVC)
    set(PROJECT_WARNINGS_FLAGS -w)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(PROJECT_WARNINGS_FLAGS ${CLANG_WARNINGS})
  else()
    set(PROJECT_WARNINGS_FLAGS ${GCC_WARNINGS})
  endif()
else()
  log_option_disabled("warning flags")
endif()


# === Set Warnings as Error Flag ===
if(OPTIONS_WARNINGS_AS_ERRORS)
  log_option_enabled("warning as error")
  set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)

  if(MSVC)
    set(PROJECT_WARNINGS_FLAGS -w)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(PROJECT_WARNINGS_FLAGS ${CLANG_WARNINGS})
  else()
    set(PROJECT_WARNINGS_FLAGS ${GCC_WARNINGS})
  endif()
else()
  log_option_disabled("warning as error")
endif()


# *****************************************************************************
# Library Configuration
# *****************************************************************************
target_sources(canary_lib
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/protocol/network_message.cpp
    ${CMAKE_CURRENT_LIST_DIR}/crypt/rsa.cpp
    ${CMAKE_CURRENT_LIST_DIR}/crypt/xtea.cpp
)

target_include_directories(canary_lib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
add_library(canary_lib::common_lib ALIAS canary_lib)

target_include_directories(canary_lib SYSTEM
                          PUBLIC
                            ${CANARY_LIB_EXTERNAL_PATH}
                            ${GMP_INCLUDE_DIR}
)

target_link_libraries(canary_lib
  PUBLIC
    doctest::doctest
    flatbuffers
    spdlog::spdlog
    ${GMP_LIBRARIES}
)

target_compile_options(canary_lib PRIVATE ${PROJECT_WARNINGS_FLAGS})
