#pragma once
#include <iostream>
#include <chrono>

#define time Time::get()

// ����� � �������������
// 0.001 (�) = 1 (��)

class Time final
{
private:
	const double maxFPS = 55.0;
	const double maxPeriod = 1000.0 / maxFPS;

	std::chrono::steady_clock::time_point previousTime;
	std::chrono::steady_clock::time_point currentTime;
	double deltaTime;

	Time() = default;
	~Time() = default;

public:
	Time(const Time&) = delete;
	Time& operator = (const Time&) = delete;

	static Time& get() 
	{
		static Time object;
		return object;
	}

	void start()
	{
		previousTime = std::chrono::high_resolution_clock::now();
	}

	void update()
	{
		currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<double, std::milli>(currentTime - previousTime).count();
	}

	bool �heckFPS()
	{
		if (deltaTime >= maxPeriod)
		{
		//	std::cout << maxPeriod << " >= " << deltaTime << std::endl;
			previousTime = currentTime;
			return true;
		}
		else
		{
		//	std::cout << maxPeriod << " < " << deltaTime << std::endl;
			return false;
		}
	}

	double getDeltaTime() { return deltaTime; }
};