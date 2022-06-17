cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(project_name OpenGL-Project)

project(${project_name})

add_executable(
	${project_name}
	
	source/main.cpp

	source/System/Parser.h
	source/System/Parser.cpp

	source/System/Time.h
	source/System/Time.cpp

	source/System/Window.h
	source/System/Window.cpp

	source/System/InputHandler.h
	source/System/InputHandler.cpp

	external/stb_image.h

	source/Renderer/ShaderProgram.h
	source/Renderer/ShaderProgram.cpp
	source/Renderer/Texture.h
	source/Renderer/Texture.cpp

	source/Renderer/Sprite.h
	source/Renderer/Sprite.cpp

	source/Renderer/IndexBuffer.h
	source/Renderer/VertexBuffer.h
	source/Renderer/VertexArray.h
	source/Renderer/VertexBufferLayout.h

	source/Resources/ResourceManager.h
	source/Resources/ResourceManager.cpp

	source/Game/ObjectsManager.h
	source/Game/ObjectsManager.cpp
	source/Game/GameObject.h
	source/Game/GameObject.cpp

	source/Game/Components/Component.h
	source/Game/Components/TransformComponent.h
	source/Game/Components/TransformComponent.cpp
	source/Game/Components/PhysicComponent.h
	source/Game/Components/PhysicComponent.cpp
	source/Game/Components/RenderComponent.h
	source/Game/Components/RenderComponent.cpp

	source/Game/Components/BehaviourComponent.h
	source/Game/Components/BehaviourComponent.cpp

	source/Game/Components/CameraComponent.h
	source/Game/Components/CameraComponent.cpp
	source/Game/Components/HeroComponent.h

	source/Physic/IShape.h
	source/Physic/Circle.h
	source/Physic/Circle.cpp
	source/Physic/Rectangle.h
	source/Physic/Rectangle.cpp

	source/Physic/CollisionHandler.h
	source/Physic/CollisionHandler.cpp
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

include_directories(external/rapidjson/include)