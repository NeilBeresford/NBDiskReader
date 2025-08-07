# ----------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------

# project names
project(NBDiskReader VERSION 1.0.0 LANGUAGES CXX)

# Add files
file(GLOB_RECURSE SOURCES "src/*.cpp" "inc/*.h" "src/modules/*.cpp" "inc/modules/*.h")

# Add the executable
add_executable(NBDiskReader ${SOURCES})

# targets for the executable
target_include_directories(NBDiskReader PRIVATE "inc" "inc/Modules" )
target_link_libraries(NBDiskReader PUBLIC NBLibrary)

# Doxgen for documentation generation
find_package(Doxygen REQUIRED)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_target(DocNBDiskReader ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
else()
    message(WARNING "Doxygen not found, documentation will not be generated.")
endif()

# End of the CMakeLists.txt file
