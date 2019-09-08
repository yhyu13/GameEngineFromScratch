#pragma once
#include "IApplication.hpp"
#include "EngineException.hpp"
#include "Utility.hpp"
#include "Timer.hpp"

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

        void OutputDebugFPS()
        {
            static int nbFrames = 0;
            nbFrames++;
            if (timer.Mark() >= 1.0)
            {
                std::wcout << wStr(1000.0/nbFrames) + L" ms/frame" << std::endl;
                nbFrames = 0;
                timer.Reset();
            }
        }

    protected:
        // Flag if need quit the main loop of the application
        static bool m_bQuit;
        GfxConfiguration m_Config;
        Timer timer;
    };
}

