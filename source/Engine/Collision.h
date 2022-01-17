#pragma once
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

using namespace std;
using namespace glm;

float dot(const vec2& first, const vec2& second) { return first.x * second.x + first.y * second.y; }

class IShape
{
public:
	virtual vec2 farthestPointInDirection(const vec2& direction) const = 0;
};

class Circle : public IShape
{
public:
	float radius;
	vec2 center;

	Circle(float radius, const vec2& center) : radius(radius), center(center) {}

	vec2 farthestPointInDirection(const vec2& direction) const override
	{
		float angle = atan2(direction.y, direction.x);
		return vec2(center.x + (radius * cos(angle)), center.y + (radius * sin(angle)));
	}

	void setCenter(const vec2& center) { this->center = center; }
};

class Rectangle : public IShape
{
public:
	vector<vec2> points;

	Rectangle() = default;

	vec2 farthestPointInDirection(const vec2& direction) const 
	{
		float farthestDistance = -numeric_limits<float>::infinity();
		float distanceInDirection;
		vec2 farthestPoint(0.f, 0.f);
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
	vector<vec2> points;

public:
	Peplix() = default;

	void add(const vec2& point) { points.push_back(point); }

	bool calculateDirection(vec2& result)
	{
		vec2 a = points[points.size() - 1];
		vec2 ao = - a;
		
		if (points.size() == 3)
		{
			vec2 b = points[1];
			vec2 c = points[0];
			vec2 ab = b - a;
			vec2 ac = c - a;
			vec2 abPerp(ab.y, -ab.x);

			if (dot(abPerp, c) >= 0) 
				abPerp = -abPerp;
			
			if (dot(abPerp, ao) > 0)
			{
				points.erase(points.begin());
				result = abPerp;
				return true;
			}

			vec2 acPerp(ac.y, -ac.x);
		
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

		vec2 b = points[0];
		vec2 ab = b - a;
		vec2 abPerp(ab.y, -ab.x);

		if (dot(abPerp, ao) <= 0)
			abPerp = -abPerp;

		result = abPerp;
		return true;
	}
};

vec2 support(const IShape& a, const IShape& b, const vec2& direction)
{
	return a.farthestPointInDirection(direction) - b.farthestPointInDirection(-direction);
}

bool calculateCollision(IShape& a, IShape& b)
{
	Peplix simplex;
	vec2 direction(0, 1);
	
	simplex.add(support(a, b, direction));
	direction = -direction;

	bool repeat = true;

	while (repeat)
	{
		vec2 supportPoint = support(a, b, direction);

		if (dot(supportPoint, direction) <= 0)
			return false;
		
		simplex.add(supportPoint);
		repeat = simplex.calculateDirection(direction);
	}
	return true;
}