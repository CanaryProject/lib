# *****************************************************************************
# Lib Externals
# *****************************************************************************
set(CANARY_LIB_3RD_PARTY_PATH             ${CMAKE_CURRENT_LIST_DIR}/3rd)
set(CANARY_LIB_EXTERNAL_PATH              ${CMAKE_CURRENT_LIST_DIR}/external)
set(CANARY_LIB_EXTERNAL_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/build/lib/external)

# *****************************************************************************
# CanaryLib
# *****************************************************************************
add_library(canary_lib "")
target_sources(canary_lib
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/protocol/network_message.cpp
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
)

add_subdirectory(3rd)

target_link_libraries(canary_lib
  PUBLIC
    spdlog::spdlog
    flatbuffers
)