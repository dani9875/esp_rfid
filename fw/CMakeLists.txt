# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set(GIT_BRANCH "unknown" CACHE STRING "Current Git branch" FORCE)
set(GIT_COMMIT "unknown" CACHE STRING "Current Git commit" FORCE)

# Get the branch and commit information using Git
execute_process(
    COMMAND "git" "rev-parse" "--abbrev-ref" "HEAD"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND "git" "rev-parse" "HEAD"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_compile_definitions(GIT_BRANCH="${GIT_BRANCH}")
add_compile_definitions(GIT_COMMIT="${GIT_COMMIT}")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)


project(esp_rfid)
idf_build_set_property(COMPILE_OPTIONS "-fdiagnostics-color=always" APPEND)

