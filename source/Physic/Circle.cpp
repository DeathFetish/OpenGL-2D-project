#include "Circle.h"

Circle::Circle(const glm::vec2& center, const float radius)
	: radius(radius), center(center) {}

glm::vec2 Circle::farthestPointInDirection(const glm::vec2& direction) const 
{
	float angle = atan2(direction.y, direction.x);
	return glm::vec2(center.x + (radius * cos(angle)), center.y + (radius * sin(angle)));
}

void Circle::recalculateIShape(const glm::vec2& position, const float rotation)
{
	center = position;
}

IShape* Circle::clone()
{
	return new Circle(center, radius);
}