get_filename_component(LIBRARY_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" LIBRARY_NAME "${LIBRARY_NAME}")
string(TOUPPER "${LIBRARY_NAME}" LIBRARY_UPPER_NAME)

# add the library that runs
add_library("${LIBRARY_NAME}" versions.c)

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

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories("${LIBRARY_NAME}"
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
install(FILES "versions.h" DESTINATION "include")
