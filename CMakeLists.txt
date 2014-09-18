cmake_minimum_required(VERSION 2.8)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

project(acmplayer)
find_package(LIBFALLTERGEIST REQUIRED)

if(NOT LIBFALLTERGEIST_FOUND)
  message(FATAL_ERROR "libfalltergeist library not found")
endif(NOT LIBFALLTERGEIST_FOUND)
include_directories(${LIBFALLTERGEIST_INCLUDE_DIR})

find_package(SDL REQUIRED)
if(NOT SDL_FOUND)
  message(FATAL_ERROR "SDL library not found")
endif(NOT SDL_FOUND)
include_directories(${SDL_INCLUDE_DIR})

find_package(SDL_mixer REQUIRED)
if(NOT SDLMIXER_FOUND)
  message(FATAL_ERROR "SDL_mixer library not found")
endif(NOT SDLMIXER_FOUND)
include_directories(${SDLMIXER_INCLUDE_DIR})

set(SOURCES
    main.cpp
)
add_definitions(
  -std=c++11
  -Wall
  -Wextra
  -pedantic
  -Wno-unused-parameter
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror -pedantic-errors")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE DEBUG)
endif(NOT CMAKE_BUILD_TYPE)

add_executable(acmplayer ${SOURCES})
target_link_libraries(acmplayer ${SDL_LIBRARY} ${LIBFALLTERGEIST_LIBRARY} ${SDLMIXER_LIBRARY})