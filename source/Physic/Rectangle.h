#pragma once
#include <vector>
#include <glm/vec2.hpp>

#include "IShape.h"

class Rectangle : public IShape
{
public:
	std::vector<glm::vec2> points;
	glm::vec2 rotationPoint;

	Rectangle(const std::vector<glm::vec2>& points, const glm::vec2& rotationPoint);

	glm::vec2 farthestPointInDirection(const glm::vec2& direction) const;

	virtual void recalculateIShape(const glm::vec2& position, const float rotation) override;

	virtual IShape* clone() override;
};