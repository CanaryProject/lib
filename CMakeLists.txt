# *****************************************************************************
# CanaryLib
# *****************************************************************************
add_library(canary_lib "")
target_sources(canary_lib
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/protocol/general.cpp
)
target_include_directories(canary_lib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
add_library(canary_lib::canary_lib ALIAS canary_lib)

#target_include_directories(canary_lib SYSTEM)
