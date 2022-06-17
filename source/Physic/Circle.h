#pragma once
#include <glm/vec2.hpp>
#include "IShape.h"

class Circle : public IShape
{
private:
	float radius;
	glm::vec2 center;

public:
	Circle(const glm::vec2& center, const float radius);

	glm::vec2 farthestPointInDirection(const glm::vec2& direction) const override;

	virtual void recalculateIShape(const glm::vec2& position, const float rotation) override;

	virtual IShape* clone() override;
};