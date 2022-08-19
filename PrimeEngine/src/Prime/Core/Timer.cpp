#include "pch.h"
#include "Timer.h"

Timer::Timer()
{
	m_last = std::chrono::high_resolution_clock::now();
}

float Timer::Mark()
{
	const auto old = m_last;
	m_last = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> frameTime = m_last - old;
	return frameTime.count();
}

float Timer::Peek()
{
	return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - m_last).count();
}
