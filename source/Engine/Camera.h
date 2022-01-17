#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

#include "Resources/WindowManager.h"
#include "Resources/ResourceManager.h"

using namespace glm;

class Camera
{
private:
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 viewProjectionMatrix;
	
	vec3 position;
	vec2 velocity;
	
	vec2 maxPositionXY;
	vec2 minPositionXY;
	
	float speed = 0.5;
	
	void recalculateViewMatrix() 
	{
		viewMatrix = translate(mat4(1.0f), position);
		viewMatrix = inverse(viewMatrix);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}
public:
	
	Camera(const Camera&) = delete;
	Camera& operator = (const Camera&) = delete;
	~Camera() = default;

	Camera(const vec3& position, const vec2& leftDownMapPoint, const vec2& rightTopMapPoint)
		: projectionMatrix(ortho(0.f, static_cast<float>(windowManger.screenSize.x), 0.f, static_cast<float>(windowManger.screenSize.y), -1.f, 1.f)),
		  position(position),
		  minPositionXY(leftDownMapPoint),
		  maxPositionXY(rightTopMapPoint - windowManger.screenSize)
	{
		velocity.x = 0;
		velocity.y = 0;
		recalculateViewMatrix();
	}

	void update(vector<bool>& keys, const vec2& cameraCenter)
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

	vec3 getPosition() const { return position; }

	mat4 getViewProjectionMatrix() const { return viewProjectionMatrix; }
};