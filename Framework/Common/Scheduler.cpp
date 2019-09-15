#include "Scheduler.hpp"

My::Scheduler::Scheduler() noexcept
{
	m_wait_time = 0;
}

My::Scheduler::Scheduler(double time) noexcept
{
	if (time >= 0)
		m_wait_time = time * .99;
}

My::Scheduler::Scheduler(int FPS) noexcept
{
	if (FPS > 0)
		m_wait_time = 1.0 / (double)FPS * .99;
}

void My::Scheduler::SetWaitTime(double time) noexcept
{
	if (time >= 0)
		m_wait_time = time * .99;
}

void My::Scheduler::SetFPS(int FPS) noexcept
{
	if (FPS > 0)
		m_wait_time = 1.0 / (double)FPS * .99;
}

bool My::Scheduler::Available() noexcept
{
	if (m_mtx.try_lock())
	{
		m_mtx.unlock();
		return true;
	}
	else
	{
		return false;
	}
}