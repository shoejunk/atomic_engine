cmake_minimum_required(VERSION 3.14)

project(atomic_engine VERSION 0.0.1)

# Use CPM.cmake to manage dependencies
include(cmake/CPM.cmake)

# Add SFML
CPMAddPackage(
  NAME SFML
  GITHUB_REPOSITORY SFML/SFML
  GIT_TAG 2.6.x
  OPTIONS
    "SFML_BUILD_WINDOW TRUE"
    "SFML_BUILD_GRAPHICS TRUE"
    "SFML_BUILD_AUDIO TRUE"
    "SFML_BUILD_NETWORK TRUE"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.h")

# Create single src filter
source_group("src" FILES ${SOURCES} ${HEADERS})

# Create executable
add_executable(atomic_engine ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(atomic_engine
  sfml-window
  sfml-graphics
  sfml-audio
  sfml-network
  sfml-system
)

if(MSVC)
    source_group(TREE ${CMAKE_SOURCE_DIR}/src FILES ${MAIN_FILE})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
    add_compile_options(/WX)
else()
    add_compile_options(-Werror)
endif()

# Copy SFML DLLs to executable directory
add_custom_command(TARGET atomic_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-audio>
        $<TARGET_FILE:sfml-network>
        $<TARGET_FILE_DIR:atomic_engine>
)

# Copy assets folder to executable directory
add_custom_command(TARGET atomic_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:atomic_engine>/assets
)
