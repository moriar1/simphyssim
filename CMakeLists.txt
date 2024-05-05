cmake_minimum_required(VERSION 3.20)
project(simulation_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

## Libraries Configuration ##

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 2.6.x
	)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)

FetchContent_Declare(imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG v1.90.4
	)
FetchContent_MakeAvailable(SFML imgui)

FetchContent_Declare(imgui-sfml
	GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
	GIT_TAG v2.6
	)
FetchContent_Declare(implot
	GIT_REPOSITORY https://github.com/epezent/implot.git
	GIT_TAG v0.16
	)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)
FetchContent_MakeAvailable(imgui-sfml)
FetchContent_MakeAvailable(implot)

#implot
#add_library(implot
	#libs/implot/implot.cpp
	#libs/implot/implot_items.cpp
	#libs/implot/implot.h
	#libs/implot/implot_internal.h
	#)
#target_link_libraries(implot PUBLIC imgui)

add_executable(simulation
	src/main.cpp
	src/base.cpp
	src/harmonic.cpp
	src/manager.cpp
	src/pendulum.cpp
	src/spring.cpp
	src/text.cpp
	build/_deps/implot-src/implot.cpp
	build/_deps/implot-src/implot_items.cpp
	)
target_link_libraries(simulation PRIVATE ImGui-SFML::ImGui-SFML
	#implot
	)
target_include_directories(simulation PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/build/_deps/implot-src
	${PROJECT_SOURCE_DIR}/build/_deps/imgui-src
	)

#############################################################

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
