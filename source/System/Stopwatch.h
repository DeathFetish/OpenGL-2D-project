#pragma once
#include <functional>

class Stopwatch
{
public:
	Stopwatch() : isRunning(false), timeLeft(0) {}

	void update(const double delta)
	{
		if (isRunning)
		{
			timeLeft -= delta;
			if (timeLeft <= 0)
			{
				isRunning = false;
				callback();
			}
		}
	}
	void start(const double duration)
	{
		timeLeft = duration;
		isRunning = true;
	}
	void setCallback(std::function<void()> callback) { this->callback = callback; }

private:
	std::function<void()> callback;
	double timeLeft;
	bool isRunning;
};