# *****************************************************************************
# Lib Externals
# *****************************************************************************
set(CANARY_LIB_3RD_PARTY_PATH             ${CMAKE_CURRENT_LIST_DIR}/3rd)
set(CANARY_LIB_EXTERNAL_PATH              ${CMAKE_CURRENT_LIST_DIR}/external)
set(CANARY_LIB_EXTERNAL_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/build/canary-lib/external)

add_subdirectory(3rd)
add_subdirectory(src)
