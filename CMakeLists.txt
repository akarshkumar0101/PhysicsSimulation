cmake_minimum_required(VERSION 3.13.4)
project(PhysicsSimulation)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)



set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} Libraries/GL/glad.c src/main.cpp src/math/Vector.h src/math/VectorOps.h src/math/Matrix.h src/math/MatrixOps.h src/RigidModel.h src/Simulation.h src/math/Quaternion.h src/Physics.h src/math/QuaternionOps.h src/Display.h src/Shader.h src/ModelData.h src/GLFWWrapper.h src/Camera.h src/Renderer.h src/GraphicsBuffer.h)


target_link_libraries(PhysicsSimulation glfw)

target_include_directories(PhysicsSimulation PRIVATE Libraries/GL/glad/include/)
target_include_directories(PhysicsSimulation PRIVATE Libraries/GLM/glm/)
target_include_directories(PhysicsSimulation PRIVATE Libraries/ImGUI/)
target_include_directories(PhysicsSimulation PRIVATE Libraries/ImGUI/examples/)

target_include_directories(PhysicsSimulation PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(PhysicsSimulation ${OPENGL_gl_LIBRARY})
target_link_libraries(PhysicsSimulation ${OPENGL_glu_LIBRARY})

#target_link_libraries(PhysicsSimulation glfw)