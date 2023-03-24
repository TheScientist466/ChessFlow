# CMakeList.txt : CMake project for ChessFlow, include source and define
# project specific logic here.
#

file(GLOB
    src_files
    "src/*.cpp"
    "src/include/ChessFlow/*.h"
)

# Add source to this project's executable.
add_executable (ChessFlow ${src_files})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChessFlow PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(ChessFlow glfw glad glm plog imgui)
include_directories(ChessFlow PUBLIC "src/include")

file(GLOB assets "src/Assets/*.glsl" "src/Assets/*.ttf" "src/Assets/*.png" "src/Assets/*.jpg")

add_custom_target( copy_shader ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/Assets
    COMMAND ${CMAKE_COMMAND} -E copy ${assets} ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/Assets/
    COMMENT "Coppying shaders..."
)
add_dependencies(${PROJECT_NAME} copy_shader)