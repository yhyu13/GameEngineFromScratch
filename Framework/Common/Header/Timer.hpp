#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
public:
	Timer() noexcept
	{
		last = steady_clock::now();
	}
	~Timer() noexcept {};

    void Reset() noexcept
	{
		last = steady_clock::now();
	}
	float Mark() noexcept
	{
		const auto old = last;
		last = steady_clock::now();
		const duration<float> Time = last - old;
		return Time.count();
	}
private:
	std::chrono::steady_clock::time_point last;
};