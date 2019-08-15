#pragma once
#include <chrono>

class Timer
{
public:
	Timer() noexcept;
	~Timer() noexcept {};

    void Reset() noexcept;
	float Mark() noexcept;
private:
	std::chrono::steady_clock::time_point last;
};