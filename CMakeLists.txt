cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Tutorial VERSION 0.0.1 LANGUAGES C)
string(TOLOWER "${PROJECT_NAME}" PROJECT_LOWER_NAME)

set(CMAKE_C_STANDARD 90)
set(CMAKE_DEBUG_POSTFIX d)

add_library("${PROJECT_LOWER_NAME}_compiler_flags" INTERFACE)
target_compile_features("${PROJECT_LOWER_NAME}_compiler_flags" INTERFACE c_std_90)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options("${PROJECT_LOWER_NAME}_compiler_flags" INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-fsanitize=address>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3;-WX;-Za;-fsanitize=address>>"
)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

if(APPLE)
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
  set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

# configure a header file to pass the version number only
configure_file("${PROJECT_NAME}Config.h.in" "${PROJECT_NAME}Config.h")

# add the library
set(LIBRARY_NAME_0 "versions")
add_subdirectory("${LIBRARY_NAME_0}")



# add the executable
add_executable("${PROJECT_NAME}" tutorial.c)
set_target_properties("${PROJECT_NAME}" PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries("${PROJECT_NAME}" PUBLIC "${LIBRARY_NAME_0}")

set(_libs "")

find_package(ZLIB REQUIRED)
list(APPEND _libs "ZLIB::ZLIB")

find_package(OpenSSL REQUIRED)
list(APPEND _libs "OpenSSL::SSL")
list(APPEND _libs "OpenSSL::Crypto")

target_link_libraries(
        "${PROJECT_NAME}"
        PUBLIC
        "${_libs}"
)

set_target_properties(
        "${PROJECT_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        C
)

# add the binary tree to the search path for include files
# so that we will find "${PROJECT_NAME}Config.h"
target_include_directories("${PROJECT_NAME}" PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

# add the install targets
install(TARGETS "${PROJECT_NAME}" DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h"
  DESTINATION include
  )

# enable testing
enable_testing()

# does the application run
add_test(NAME Runs COMMAND "${PROJECT_NAME}" 25)

# does the usage message work?
add_test(NAME Usage COMMAND "${PROJECT_NAME}")
set_tests_properties(Usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
  )

# define a function to simplify adding tests
function(do_test target arg result)
  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
  set_tests_properties(Comp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction(do_test)

# do a bunch of result based tests
do_test("${PROJECT_NAME}" 4 "4 is 2")
do_test("${PROJECT_NAME}" 9 "9 is 3")
do_test("${PROJECT_NAME}" 5 "5 is 2.236")
do_test("${PROJECT_NAME}" 7 "7 is 2.645")
do_test("${PROJECT_NAME}" 25 "25 is 5")
do_test("${PROJECT_NAME}" -25 "-25 is [-nan|nan|0]")
do_test("${PROJECT_NAME}" 0.0001 "0.0001 is 0.01")

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
include(CPack)

# install the configuration targets
install(EXPORT "${LIBRARY_NAME_0}Targets"
  FILE "${LIBRARY_NAME_0}Targets.cmake"
  DESTINATION "lib/cmake/${LIBRARY_NAME_0}"
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}Config.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )
# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}ConfigVersion.cmake"
  VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install the configuration file
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}Config.cmake"
  DESTINATION "lib/cmake/${LIBRARY_NAME_0}"
  )

# generate the export targets for the build tree
# needs to be after the install(TARGETS ) command
export(EXPORT "${LIBRARY_NAME_0}Targets"
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}Targets.cmake"
)
