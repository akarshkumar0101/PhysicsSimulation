cmake_minimum_required(VERSION 3.13.4)
project(PhysicsSimulation)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)



set(CMAKE_CXX_STANDARD 17)

file(GLOB IMGUI_SRC
    "Libraries/ImGUI/*.h"
    "Libraries/ImGUI/*.cpp"
    "Libraries/ImGUI/examples/imgui_impl_glfw.h"
    "Libraries/ImGUI/examples/imgui_impl_glfw.cpp"
    "Libraries/ImGUI/examples/imgui_impl_opengl2.h"
    "Libraries/ImGUI/examples/imgui_impl_opengl2.cpp"
)

add_executable(${PROJECT_NAME} Libraries/GL/glad.c
        ${IMGUI_SRC}
        #Libraries/ImGUI/examples/example_glfw_opengl2/main.cpp
        src/main.cpp
        src/math/Vector.h src/math/VectorOps.h src/math/Matrix.h src/math/MatrixOps.h src/RigidModel.h src/PhysicsSimulation.h src/math/Quaternion.h src/Physics.h src/math/QuaternionOps.h src/SimulationDisplay.h src/Shader.h src/ModelGraphicsData.h src/Camera.h src/Renderer.h src/GraphicsBuffer.h src/Shader.cpp src/GraphicsBuffer.cpp src/Window.cpp src/Window.h src/Camera.cpp src/Renderer.cpp src/ModelGraphicsData.cpp)


target_link_libraries(PhysicsSimulation glfw)

target_include_directories(PhysicsSimulation PRIVATE Libraries/GL/glad/include/)
target_include_directories(PhysicsSimulation PRIVATE Libraries/GLM/glm/)
target_include_directories(PhysicsSimulation PRIVATE Libraries/ImGUI/)
target_include_directories(PhysicsSimulation PRIVATE Libraries/ImGUI/examples/)

target_include_directories(PhysicsSimulation PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(PhysicsSimulation ${OPENGL_gl_LIBRARY})
target_link_libraries(PhysicsSimulation ${OPENGL_glu_LIBRARY})

#target_link_libraries(PhysicsSimulation glfw)