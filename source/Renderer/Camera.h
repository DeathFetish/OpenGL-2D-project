#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "../Managers/WindowManager.h"
#include "../Game/GameObject.h"

using namespace glm;

class Camera
{
private:
	const GameObject* objectInFocus;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 viewProjectionMatrix;
	
	glm::vec3 position;
	glm::vec3 scale;

	glm::vec2 maxPositionXY;
	glm::vec2 minPositionXY;
	
	const float speed = 1.f;
	
	void recalculateViewMatrix() 
	{
		viewMatrix = translate(mat4(1.0f), position);
		viewMatrix = inverse(viewMatrix);
		viewMatrix = viewMatrix * glm::scale(mat4(1.0f), scale);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}
public:
	
	Camera(const Camera&) = delete;
	Camera& operator = (const Camera&) = delete;
	~Camera() = default;

	Camera(const vec2& position, const vec2& leftDownMapPoint, const vec2& rightTopMapPoint, const vec2& scale, const GameObject* objectInFocus)
		: projectionMatrix(ortho(0.f, windowManger.screenSize.x, 0.f, windowManger.screenSize.y, -100.f, 100.f)),
		position(vec3(position, 0)),
		scale(vec3(scale, 0)),
		minPositionXY(leftDownMapPoint),
		maxPositionXY(rightTopMapPoint - windowManger.screenSize),
		objectInFocus(objectInFocus)
	{
		recalculateViewMatrix();
	}

	void update()
	{
		position.x = (objectInFocus->getPosition().x + objectInFocus->getPivot().x) * scale.x - windowManger.screenSize.x * 0.5f;
		position.y = (objectInFocus->getPosition().y + objectInFocus->getPivot().y) * scale.y - windowManger.screenSize.y * 0.5f;

		position.x = clamp(position.x, minPositionXY.x, maxPositionXY.x);
		position.y = clamp(position.y, minPositionXY.y, maxPositionXY.y);

		recalculateViewMatrix();
	}

	const glm::vec3& getPosition() const { return position; }

	const glm::mat4& getViewProjectionMatrix() const { return viewProjectionMatrix; }

	glm::vec2 screenToWorldPoint(int x, int y)
	{
		return glm::vec2((position.x + x) / scale.x, (position.y + y) / scale.y);
	}
};