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

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if(USE_MYMATH)

  target_compile_definitions("${LIBRARY_NAME}" PRIVATE "USE_MYMATH")

  # first we add the executable that generates the table
  add_executable(MakeTable MakeTable.c)
  target_link_libraries(MakeTable PRIVATE "${PROJECT_LOWER_NAME}_compiler_flags")

  # add the command to generate the source code
  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Table.h"
    COMMAND MakeTable "${CMAKE_CURRENT_BINARY_DIR}/Table.h"
    DEPENDS MakeTable
  )

  # library that just does sqrt
  set(DEPENDANT_LIBRARY "SqrtLibrary")
  add_library(
          "${DEPENDANT_LIBRARY}" STATIC
          mysqrt.c
          "${CMAKE_CURRENT_BINARY_DIR}/Table.h"
  )

  set_target_properties(
          "${DEPENDANT_LIBRARY}"
          PROPERTIES
          LINKER_LANGUAGE
          C
  )

  # state that we depend on our binary dir to find Table.h
  target_include_directories("${DEPENDANT_LIBRARY}" PRIVATE
                             "${CMAKE_CURRENT_BINARY_DIR}"
                             )

  # state that SqrtLibrary need PIC when the default is shared libraries
  set_target_properties("${DEPENDANT_LIBRARY}" PROPERTIES
                        POSITION_INDEPENDENT_CODE "${BUILD_SHARED_LIBS}"
                        )

  target_link_libraries("${DEPENDANT_LIBRARY}" PUBLIC "${PROJECT_LOWER_NAME}_compiler_flags")
  target_link_libraries("${LIBRARY_NAME}" PRIVATE "${DEPENDANT_LIBRARY}")
endif()

target_link_libraries("${LIBRARY_NAME}" PUBLIC "${PROJECT_LOWER_NAME}_compiler_flags")

# define the symbol stating we are using the declspec(dllexport) when
# building on windows
target_compile_definitions("${LIBRARY_NAME}" PRIVATE "EXPORTING_MYMATH")
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
