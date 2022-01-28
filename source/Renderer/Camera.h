#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "../Managers/WindowManager.h"

using namespace glm;

class Camera
{
private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
//	glm::mat4 viewProjectionMatrix;
	
	glm::vec3 position;
	glm::vec2 velocity;
	glm::vec3 scale;

	glm::vec2 maxPositionXY;
	glm::vec2 minPositionXY;
	
	float speed = 0.5;
	
	void recalculateViewMatrix() 
	{
		viewMatrix = translate(mat4(1.0f), position);
		viewMatrix = viewMatrix * glm::scale(mat4(1.0f), scale);
		
	//	viewMatrix = inverse(viewMatrix);
	//	viewProjectionMatrix = projectionMatrix * viewMatrix;
	}
public:
	
	Camera(const Camera&) = delete;
	Camera& operator = (const Camera&) = delete;
	~Camera() = default;

	Camera(const vec3& position, const vec2& leftDownMapPoint, const vec2& rightTopMapPoint, const vec2& scale)
		: projectionMatrix(ortho(0.f, windowManger.screenSize.x, 0.f, windowManger.screenSize.y, -1.f, 1.f)),
	//	: projectionMatrix(ortho(0.f, windowManger.screenSize.x / 10, 0.f, windowManger.screenSize.y / 10, -1.f, 1.f)),
		  position(position), scale(vec3(scale, 0)),
		  minPositionXY(leftDownMapPoint),
		  maxPositionXY(rightTopMapPoint - windowManger.screenSize)
	{
		velocity.x = 0;
		velocity.y = 0;
		recalculateViewMatrix();
	}

	void update(std::vector<bool>& keys, const vec2& cameraCenter)
	{
		position.x = cameraCenter.x;
		position.y = cameraCenter.y;

		if (position.x > maxPositionXY.x)
			position.x = maxPositionXY.x;
		else if (position.x < minPositionXY.x)
			position.x = minPositionXY.x;

		if (position.y > maxPositionXY.y)
			position.y = maxPositionXY.y;
		else if (position.y < minPositionXY.y)
			position.y = minPositionXY.y;

		recalculateViewMatrix();
	}

	glm::vec3 getPosition() const { return position; }

	glm::mat4 getViewProjectionMatrix() const { return projectionMatrix * viewMatrix; }

	glm::vec2 screenToWorldPoint(int x, int y)
	{
		return glm::vec2((position.x + x) / scale.x, (position.y + y) / scale.y);
	}
};