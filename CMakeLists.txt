cmake_minimum_required(VERSION 3.19)

# set the project name and version
project(CmakeExampleWithVcpkg VERSION 0.0.1 LANGUAGES C)
string(TOLOWER "${PROJECT_NAME}" PROJECT_LOWER_NAME)

set(CMAKE_C_STANDARD 90)
set(CMAKE_VERBOSE_MAKEFILE ON)
if (NOT MSVC)
  set(CMAKE_DEBUG_POSTFIX "d")
endif ()

add_library("${PROJECT_LOWER_NAME}_compiler_flags" INTERFACE)
target_compile_features("${PROJECT_LOWER_NAME}_compiler_flags" INTERFACE "c_std_90")

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like "$<COMPILE_LANG_AND_ID:C,CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc "$<COMPILE_LANG_AND_ID:C,CXX,MSVC>")
# -g -fsanitize=address -fno-omit-frame-pointer
target_compile_options(
        "${PROJECT_LOWER_NAME}_compiler_flags"
        INTERFACE
        "$<${gcc_like}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
        "$<${msvc}:$<BUILD_INTERFACE:-W3;-WX;-Zi;-permissive->>"
)
if (NOT MSVC)
  add_compile_options("$<$<CONFIG:RELEASE>:-O3>")

  if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options("$<$<CONFIG:DEBUG>:-pedantic>")
  else ()
    add_compile_options("$<$<CONFIG:DEBUG>:-Wpedantic>")
  endif ()
endif ()

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
configure_file(
        "${PROJECT_NAME}Config.h.in"
        "${PROJECT_NAME}Config.h"
)

# add the library
set(LIBRARY_NAME_0 "versions")
add_subdirectory("${LIBRARY_NAME_0}")

# add the executable
add_executable("${PROJECT_NAME}" "main_cmake_example_with_vcpkg.c")
if (NOT MSVC)
  set_target_properties("${PROJECT_NAME}" PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
endif ()

target_link_libraries("${PROJECT_NAME}" PUBLIC "${LIBRARY_NAME_0}")

set_target_properties(
        "${PROJECT_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        C
)

# add the binary tree to the search path for include files
# so that we will find "${PROJECT_NAME}Config.h"
target_include_directories(
        "${PROJECT_NAME}"
        PUBLIC
        "${PROJECT_BINARY_DIR}"
)

# enable testing
include(CTest)
if (BUILD_TESTING)
    enable_testing()

    # does the application run
    add_test(NAME Runs COMMAND "${PROJECT_NAME}")

    # is the output correct?
    add_test(NAME Output COMMAND "${PROJECT_NAME}")
    set_tests_properties(
            Output
            PROPERTIES PASS_REGULAR_EXPRESSION
                "version:*;get_hash():*;ZLIB_VERSION:*;OPENSSL_VERSION_TEXT:*"
    )
endif (BUILD_TESTING)

# add the install targets
install(
        TARGETS "${PROJECT_NAME}"
        DESTINATION "bin"
)
install(
        FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h"
        DESTINATION "include"
)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
include(CPack)

# install the configuration targets
install(
        EXPORT "${LIBRARY_NAME_0}Targets"
        FILE "${LIBRARY_NAME_0}Targets.cmake"
        DESTINATION "lib/cmake/${LIBRARY_NAME_0}"
)

include(CMakePackageConfigHelpers)

# generate the version file for the config file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}ConfigVersion.cmake"
        VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
        COMPATIBILITY AnyNewerVersion
)

# install the configuration file
install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}Config.cmake"
        DESTINATION "lib/cmake/${LIBRARY_NAME_0}"
)

# generate the export targets for the build tree
# needs to be after the install(TARGETS ) command
export(
        EXPORT "${LIBRARY_NAME_0}Targets"
        FILE "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}Targets.cmake"
)
