#pragma once
#include <map>
#include <glm/vec2.hpp>

#include "../Managers/WindowManager.h"


class GameObject
{
protected:
	glm::vec2 position;
	glm::vec2 pivot;

public:
	const vec2& getPivot() const { return glm::vec2(0.5f * 36, 0.5f * 19); }
	const vec2& getPosition() const { return position; }
};
