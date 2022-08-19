#pragma once
#include <chrono>
class Timer
{
public:
	Timer();
	~Timer() {}
	
	// Returns the time in seconds since the last call to this function
	float Mark();
	// Shows where the timer is right now
	float Peek();

private:
	std::chrono::high_resolution_clock::time_point m_last;
};