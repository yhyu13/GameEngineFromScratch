#pragma once
#include <chrono>
#include "EngineException.hpp"

using namespace std::chrono;

namespace My
{
	class Timer
	{
	public:
		Timer()
		{
			if(std::chrono::steady_clock::is_steady == false)
				throw EngineException(_CRT_WIDE(__FILE__),__LINE__,L"C++11 std::chrono::steady_clock::is_steady() returns FALSE.");
			last = steady_clock::now();
		}
		~Timer() noexcept {};

		void Reset() noexcept
		{
			last = steady_clock::now();
		}
		double Mark() noexcept
		{
			const duration<double> Time = steady_clock::now() - last;
			return Time.count();
		}
	private:
		std::chrono::steady_clock::time_point last;
	};
}