get_filename_component(LIBRARY_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" LIBRARY_NAME "${LIBRARY_NAME}")
string(TOUPPER "${LIBRARY_NAME}" LIBRARY_UPPER_NAME)

include(GenerateExportHeader)

# add the library that runs
add_library("${LIBRARY_NAME}" "versions.c")

set(_libs "")

find_package(ZLIB REQUIRED)
list(APPEND _libs "ZLIB::ZLIB")

find_package(OpenSSL REQUIRED)
list(APPEND _libs "OpenSSL::SSL")
list(APPEND _libs "OpenSSL::Crypto")

target_link_libraries(
        "${LIBRARY_NAME}"
        PUBLIC
        "${_libs}"
)

target_include_directories(
        "${LIBRARY_NAME}"
        INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include>"
)

target_link_libraries("${LIBRARY_NAME}" PUBLIC "${PROJECT_LOWER_NAME}_compiler_flags")

set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        C
)

set(_export_file "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")


# tests
include(CTest)
if (BUILD_TESTING)
  add_executable("${LIBRARY_NAME}Test" "${LIBRARY_NAME}_test.c")

  set(REXO_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/downloaded_headers")
  file(DOWNLOAD
          https://raw.githubusercontent.com/christophercrouzet/rexo/bebf252/include/rexo.h
          "${REXO_INCLUDE_DIRS}/rexo.h"
       EXPECTED_HASH
          SHA256=229e7a5eec440c18501c7cda345c8b8feaebb4c3a8c21db70ef5144d1b2f9ba9)
  # Remove ^ and uncomment this next line once https://github.com/microsoft/vcpkg/pull/18895 merges
  # find_path(REXO_INCLUDE_DIRS "rexo.h")
  target_include_directories("${LIBRARY_NAME}Test" PRIVATE "${REXO_INCLUDE_DIRS}")
  target_link_libraries("${LIBRARY_NAME}Test" PUBLIC "${_libs}" "${LIBRARY_NAME}")

  add_test(NAME "${LIBRARY_NAME}Test" COMMAND "${LIBRARY_NAME}Test")
endif (BUILD_TESTING)


# setup the version numbering
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "1.0.0")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "1")

# install rules
set(installable_libs "${LIBRARY_NAME}" "${PROJECT_LOWER_NAME}_compiler_flags")
if(TARGET "${DEPENDANT_LIBRARY}")
  list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif()
install(TARGETS ${installable_libs}
        DESTINATION "lib"
        EXPORT "${LIBRARY_NAME}Targets")
install(FILES "${_export_file}" "versions.h" DESTINATION "include")
