cmake_minimum_required(VERSION 2.8)

project(starconquest)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(NOT UNIX)
    set(SDL2_LIBRARY sdl2)
    set(SDL2_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(SDL2_IMAGE_LIBRARY sdl2_image)
    set(SDL2_IMAGE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2_IMAGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(SDL2TTF_LIBRARY sdl2ttf)
    set(SDL2TTF_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2TTF_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(SDL2_MIXER_LIBRARY sdl2ttf)
    set(SDL2_MIXER_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2_MIXER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(Intl_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/gettext/include )

    #message("${PROJECT_SOURCE_DIR}/deps/gettext/lib/")

    link_directories(${SDL2_DIR}/lib/ ${PROJECT_SOURCE_DIR}/deps/gettext/lib/ )

endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image)
find_package(SDL2_mixer)
find_package(SDL2_gfx)
find_package(SDL2TTF)
find_package(ZLIB)
#find_package(Intl)




#FIND_PACKAGE(Boost COMPONENTS locale)
message( "${Intl_INCLUDE_DIR}")


INCLUDE_DIRECTORIES()
#file(GLOB_RECURSE SRC battle *.cpp)
#file(GLOB_RECURSE SRC character *.cpp)
#file(GLOB_RECURSE SRC dialogs *.cpp)
#file(GLOB_RECURSE SRC engine/* *.cpp)

file(GLOB_RECURSE SRC
    engine/*
    src/*.cpp)
#message ("${SRC}")

set (GAME_VERSION_MAJOR 0)
set (GAME_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}" ${SDL2_INCLUDE_DIR} ${GETTEXT_INCLUDE_DIR} ${Intl_INCLUDE_DIR})


add_executable(${PROJECT_NAME} "src/starconquest.cpp" ${SRC})

if( UNIX)
option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)

    include(googletest)
    include(GoogleTest)
    fetch_googletest(
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_BINARY_DIR}/googletest
        )
    enable_testing()
    add_subdirectory(tests)
    #add_executable(SimpleTest )


endif()
endif()

target_link_libraries(${PROJECT_NAME} )
if(NOT UNIX)
    target_link_libraries(${PROJECT_NAME} mingw32 SDL2main)
endif()
target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} SDL2_ttf)
target_link_libraries(${PROJECT_NAME} SDL2_image)
target_link_libraries(${PROJECT_NAME} SDL2_mixer)
if(UNIX)
target_link_libraries(${PROJECT_NAME} SDL2_gfx)
endif()
#target_link_libraries(${PROJECT_NAME} z)

#target_link_libraries(${PROJECT_NAME} Intl)
target_link_libraries(${PROJECT_NAME} pthread)

#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})


add_custom_command(TARGET starconquest PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/fonts/ $<TARGET_FILE_DIR:starconquest>/fonts/)
add_custom_command(TARGET starconquest PRE_BUILD
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                      ${CMAKE_SOURCE_DIR}/locale/ $<TARGET_FILE_DIR:starconquest>/locale/)
add_custom_command(TARGET starconquest PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/music/ $<TARGET_FILE_DIR:starconquest>/music/)

add_custom_command(TARGET starconquest PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/images/ $<TARGET_FILE_DIR:starconquest>/images/)
add_custom_command(TARGET starconquest PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/data/ $<TARGET_FILE_DIR:starconquest>/data/)


                add_custom_command(TARGET starconquest POST_BUILD
                                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                                       ${CMAKE_SOURCE_DIR}/deps/SDL2/bin/ $<TARGET_FILE_DIR:starconquest>)
INSTALL(TARGETS starconquest DESTINATION starconquest)
INSTALL(DIRECTORY fonts DESTINATION starconquest )
INSTALL(DIRECTORY data DESTINATION starconquest )
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0 (>= 2.0), libsdl2-ttf-2.0-0 (>= 2.0), libsdl2-image-2.0-0 (>= 2.0), libsdl2-gfx-2.0-0 (>= 2.0), libsdl2-mixer-2.0-0 (>= 2.0)")
set(CPACK_PACKAGE_VERSION "${GAME_VERSION_MAJOR}.${GAME_VERSION_MINOR}")
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Stefan Lüdtke") #required
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "sota is a 2d isometric economy simulation game.")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")
INCLUDE(CPack)

