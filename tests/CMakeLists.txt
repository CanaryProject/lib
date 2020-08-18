cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

enable_testing()

project(canary-lib-tests)

add_subdirectory(src)

# === Benchmarking ===
# -DOPTIONS_ENABLE_BENCHMARKING=ON
if(OPTIONS_ENABLE_BENCHMARKING)
  log_option_enabled("benchmarking")
  add_subdirectory(benchmarking)
else()
  log_option_disabled("benchmarking")
endif()

# Make test executable
add_executable(canary-lib-tests main.cpp ${CANARY_LIB_TEST_SRC})

target_compile_definitions(canary-lib-tests PUBLIC -DUNIT_TESTING -DDEBUG_LOG)

target_link_libraries(canary-lib-tests canary_lib::common_lib)

set_target_properties(canary-lib-tests
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
