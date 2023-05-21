cmake_minimum_required(VERSION 3.19)

project(VSTFX)

# -------- CMake meta --------

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_COLOR_MAKEFILE ON)

# -------- Options --------

option(WITH_GUI "Build with GUI" ON)

# -------- Compiler stuff --------

if(MSVC)
    add_definitions(/D _CRT_SECURE_NO_WARNINGS)
endif()

# static compile
if (MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static -s")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static -s")
endif()

# -------- Project Source Dir --------

set(VSTFX_SOURCE_DIR src)

# -------- Project Sources --------

file(GLOB PROJECT_SOURCES
    "${VSTFX_SOURCE_DIR}/*.cpp"
    "${VSTFX_SOURCE_DIR}/*.hpp"
    "${VSTFX_SOURCE_DIR}/*.def"
    "${VSTFX_SOURCE_DIR}/*.rc"

    "${VSTFX_SOURCE_DIR}/vst.h"
)

# -------- GUI Sources --------

if(WITH_GUI)
    add_definitions(-DWITH_GUI)

    # use Dear Imgui

    include_directories(
	vendor/imgui_patched
	vendor/imgui_patched/backends
	vendor/SDL/include
    )

    set(IMGUI_SOURCES
	vendor/imgui_patched/imgui.cpp
	vendor/imgui_patched/imgui_draw.cpp
	vendor/imgui_patched/imgui_widgets.cpp
	vendor/imgui_patched/imgui_tables.cpp
	vendor/imgui_patched/fileutils.cpp
	vendor/imgui_patched/utfutils.cpp
	vendor/imgui_patched/imgui_demo.cpp
	vendor/imgui_patched/backends/imgui_impl_sdl.cpp
	vendor/imgui_patched/backends/imgui_impl_sdlrenderer.cpp
    )

    list(APPEND PROJECT_SOURCES
	${IMGUI_SOURCES}
    )

    # use SDL2
    set(SDL_STATIC ON CACHE BOOL "Force statically-linked SDL" FORCE)
    set(SDL_LIBC ON CACHE BOOL "Tell SDL that we want it to use our C runtime (required for proper static linking)" FORCE)
    add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

    # compile images

    include(cmake/bin2h.cmake)

    bin2h(
		SOURCE_FILE "${VSTFX_SOURCE_DIR}/gui/res/logo.png"
		HEADER_FILE "${VSTFX_SOURCE_DIR}/gui/res/include/logo.cpp"
		VARIABLE_NAME "imgTest"
    )

    file(GLOB PROJECT_GUI_SOURCES
	"${VSTFX_SOURCE_DIR}/gui/res/include/*.cpp"
	"${VSTFX_SOURCE_DIR}/gui/*.cpp"
	"${VSTFX_SOURCE_DIR}/gui/*.hpp"
	"vendor/stb_image.h"
    )

    list(APPEND PROJECT_SOURCES
	${PROJECT_GUI_SOURCES}
    )
endif()

# -------- Link target --------

add_library(VSTFX MODULE ${PROJECT_SOURCES})
set_target_properties(VSTFX PROPERTIES PREFIX "")

# add SDL2
if(WITH_GUI)
    target_link_libraries(VSTFX PRIVATE SDL2-static SDL2main shlwapi)
else()
    target_link_libraries(VSTFX PRIVATE shlwapi)
endif()

#install(TARGETS VSTFX
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#)
