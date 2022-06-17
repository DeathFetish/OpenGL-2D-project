#pragma once
#include <glm/vec2.hpp>

class IShape
{
public:
	virtual glm::vec2 farthestPointInDirection(const glm::vec2& direction) const = 0;
	virtual void recalculateIShape(const glm::vec2& position, const float rotation) = 0;
	virtual IShape* clone() = 0;
};