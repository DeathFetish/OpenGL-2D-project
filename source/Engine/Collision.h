#pragma once
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

float dot(const glm::vec2& first, const glm::vec2& second) { return first.x * second.x + first.y * second.y; }

class IShape
{
public:
	virtual glm::vec2 farthestPointInDirection(const glm::vec2& direction) const = 0;
};

class Circle : public IShape
{
private:
	float radius;
	glm::vec2 center;

public:
	Circle(float radius, const glm::vec2& center) : radius(radius), center(center) {}

	glm::vec2 farthestPointInDirection(const glm::vec2& direction) const override
	{
		float angle = atan2(direction.y, direction.x);
		return glm::vec2(center.x + (radius * cos(angle)), center.y + (radius * sin(angle)));
	}

	void setCenter(const glm::vec2& center) { this->center = center; }
};

class Rectangle : public IShape
{
public:
	std::vector<glm::vec2> points;

	Rectangle(std::vector<glm::vec2> points) : points(points) {};

	glm::vec2 farthestPointInDirection(const glm::vec2& direction) const
	{
		float farthestDistance = -std::numeric_limits<float>::infinity();
		float distanceInDirection;
		glm::vec2 farthestPoint(0.f, 0.f);
		for (auto point = points.cbegin(); point != points.cend(); point++)
		{
			distanceInDirection = dot(*point, direction);
			if (distanceInDirection > farthestDistance) 
			{
				farthestPoint = *point;
				farthestDistance = distanceInDirection;
			}
		}
		return farthestPoint;
	}
};

class Peplix
{
private:
	std::vector<glm::vec2> points;

public:
	Peplix() = default;

	void add(const glm::vec2& point) { points.push_back(point); }

	bool calculateDirection(glm::vec2& result)
	{
		glm::vec2 a = points[points.size() - 1];
		glm::vec2 ao = -a;
		
		if (points.size() == 3)
		{
			glm::vec2 b = points[1];
			glm::vec2 c = points[0];
			glm::vec2 ab = b - a;
			glm::vec2 ac = c - a;
			glm::vec2 abPerp(ab.y, -ab.x);

			if (dot(abPerp, c) >= 0) 
				abPerp = -abPerp;
			
			if (dot(abPerp, ao) > 0)
			{
				points.erase(points.begin());
				result = abPerp;
				return true;
			}

			glm::vec2 acPerp(ac.y, -ac.x);
		
			if (dot(acPerp, b) >= 0)
				acPerp = -acPerp;

			if (dot(acPerp, ao) > 0)
			{
				points.erase(points.begin() + 1);
				result = acPerp;
				return true;
			}
			return false;
		}

		glm::vec2 b = points[0];
		glm::vec2 ab = b - a;
		glm::vec2 abPerp(ab.y, -ab.x);

		if (dot(abPerp, ao) <= 0)
			abPerp = -abPerp;

		result = abPerp;
		return true;
	}
};

glm::vec2 support(const IShape& a, const IShape& b, const glm::vec2& direction)
{
	return a.farthestPointInDirection(direction) - b.farthestPointInDirection(-direction);
}

bool calculateCollision(IShape& a, IShape& b)
{
	Peplix simplex;
	glm::vec2 direction(0, 1);
	
	simplex.add(support(a, b, direction));
	direction = -direction;

	bool repeat = true;

	while (repeat)
	{
		glm::vec2 supportPoint = support(a, b, direction);

		if (dot(supportPoint, direction) <= 0)
			return false;
		
		simplex.add(supportPoint);
		repeat = simplex.calculateDirection(direction);
	}
	return true;
}