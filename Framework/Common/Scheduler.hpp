#pragma once
#include <mutex>
#include <future>
#include "Timer.hpp"

namespace My
{
    class Scheduler
    {
        public:
            Scheduler() noexcept;
            explicit Scheduler(double time) noexcept;
            explicit Scheduler(int FPS) noexcept;
            Scheduler(const Scheduler&) = delete;
            Scheduler( Scheduler&& other ) = delete;

        private:
            Timer m_timer;
            std::mutex m_mtx;
            double m_wait_time;

        public:
            void SetWaitTime(double time) noexcept;
            void SetFPS(int FPS) noexcept;
            bool Available() noexcept;

            /**
             *  \brief Blocking event firing template that invokes the function in the main thread.
             *  \return Function return type must not be void. Better using lambda function (e.g.,[](func();return 0;)) to wrap up a void function.
            */
            template< class Function, class... Args >
            std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)> Fire( Function&& func, Args&&... args )
            {
                m_mtx.lock();
                m_timer.Reset();
                std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)> ret = func(args...);
                while(m_timer.Mark() < m_wait_time) {};
                m_mtx.unlock();
                return ret;
            }

            /**
             *  \brief Blocking event firing template that invokes the function in a another thread.
             *  \return Function return type must not be void. Better using lambda function (e.g.,[](func();return 0;)) to wrap up a void function.
            */
            template< class Function, class... Args >
            std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)> FireAsync( Function&& func, Args&&... args )
            {
                m_mtx.lock();
                m_timer.Reset();
                std::future<std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>> result = std::async(std::launch::async, func, args...);
                std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)> ret = result.get();
                while(m_timer.Mark() < m_wait_time) {};
                m_mtx.unlock();
                return ret;
            }
    };
}