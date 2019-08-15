#include "Timer.hpp"

using namespace std::chrono;

Timer::Timer() noexcept
{
	last = steady_clock::now();
}

void Timer::Reset() noexcept
{
    last = steady_clock::now();
}

float Timer::Mark() noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> Time = last - old;
	return Time.count();
}
