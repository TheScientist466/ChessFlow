cmake_minimum_required(VERSION 3.8)

project(extlibs)

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(plog)

set(IMGUI_DIR imgui)
add_library(imgui
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_DIR}/backends/imgui_impl_glfw.h
	${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
	${IMGUI_DIR}/backends/imgui_impl_opengl3.h
)
target_include_directories(imgui PUBLIC "imgui")
target_include_directories(imgui PUBLIC "${IMGUI_DIR}/backends")
target_link_libraries(imgui PUBLIC glfw)

target_include_directories(imgui PUBLIC "stb")