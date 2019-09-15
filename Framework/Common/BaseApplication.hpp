#pragma once
#include "IApplication.hpp"
#include "EngineException.hpp"
#include "Utility.hpp"
#include "Timer.hpp"
#include "Scheduler.hpp"

namespace My {
    class BaseApplication : implements IApplication
    {
    public:
        BaseApplication() = delete;
        BaseApplication( const BaseApplication& ) = delete;
        BaseApplication& operator=( const BaseApplication& ) = delete;

        BaseApplication(GfxConfiguration& cfg);
        virtual int Initialize();
        virtual void Finalize();
        // One cycle of the main loop
        virtual void Tick();

        virtual bool IsQuit();

        inline GfxConfiguration& GetConfiguration() { return m_Config; };
        virtual void ShowMessage( const std::wstring& title,const std::wstring& message ) const;

    protected:
        virtual void OnDraw() {};

        static void OutputDebugFPS();

    protected:
        // Flag if need quit the main loop of the application
        static bool m_bQuit;
        static int m_nbFrames;
        static Scheduler m_drawScheduler;
        GfxConfiguration m_Config;
    private:
        static Timer m_timer;
    };
}

