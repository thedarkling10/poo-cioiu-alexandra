cmake_minimum_required(VERSION 3.26)

# Project name and executable name
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

# Define Project
project(${MAIN_PROJECT_NAME})

# Set C++ standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include additional cmake scripts
include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

# Headers and Sources
set(PROJECT_HEADERS
        GameEntity.h
        Character.h
        Enemy.h
        Item.h
        Prop.h
        Obstacle.h
        Exceptions.h
)

set(PROJECT_SOURCES
        main.cpp
        GameEntity.cpp
        Character.cpp
        Enemy.cpp
        Item.cpp
        Prop.cpp
        Obstacle.cpp
        InteractiveObject.cpp
        InteractiveObject.h
        NPC.cpp
        NPC.h
)

# Add executable target
add_executable(${MAIN_EXECUTABLE_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})

# Include necessary directories
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/libs)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE generated/include)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ext/include)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE libs/termcolor)

# Optional: Link system libraries or external libraries here if required
# Example: target_link_libraries(${MAIN_EXECUTABLE_NAME} SomeLibrary)

# Set compiler flags for sanitizers and warnings
set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

# Install binary and resources
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

# Resource copying
# Adjust or list all required files here to copy to the binary location
copy_files(
        FILES tastatura.txt
        COPY_TO_DESTINATION
        TARGET_NAME ${MAIN_EXECUTABLE_NAME}
)