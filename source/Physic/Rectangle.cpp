#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Rectangle.h"

Rectangle::Rectangle(
	const std::vector<glm::vec2>& points,
	const glm::vec2& rotationPoint) : points(points), rotationPoint(rotationPoint) {}

glm::vec2 Rectangle::farthestPointInDirection(const glm::vec2& direction) const
{
	float farthestDistance = -std::numeric_limits<float>::infinity();
	float distanceInDirection;

	glm::vec2 farthestPoint(0.f, 0.f);
	for (auto point = points.cbegin(); point != points.cend(); point++)
	{
		distanceInDirection = glm::dot(*point, direction);
		if (distanceInDirection > farthestDistance)
		{
			farthestPoint = *point;
			farthestDistance = distanceInDirection;
		}
	}
	return farthestPoint;
}

void Rectangle::recalculateIShape(const glm::vec2& position, const float rotation)
{
	glm::mat4 rotationMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));

	for (unsigned int i = 0; i < 4; ++i)
		points[i] = glm::vec2(rotationMat * glm::vec4(points[i] - rotationPoint - position, 0, 0)) + rotationPoint + position;
}

IShape* Rectangle::clone()
{
	return new Rectangle(points, rotationPoint);
}