#pragma once
#include "IApplication.hpp"
#include "GfxConfiguration.h"
#include "EngineException.hpp"

namespace My {
    class BaseApplication : implements IApplication {
        public:
            // Disable default constructor
            BaseApplication() = delete;
            BaseApplication( const BaseApplication& ) = delete;
	        BaseApplication& operator=( const BaseApplication& ) = delete;
            
            explicit BaseApplication(GfxConfiguration& cfg);
            
            virtual int Initialize();
            virtual void Finalize();
            virtual void Tick();
            virtual bool IsQuit();

            virtual void ShowMessage( const std::wstring& title,const std::wstring& message ) const;

        protected:
            void SetIsQuit(bool bQuit) {
                m_bQuit = bQuit;
            }

        protected:
            static bool m_bQuit;          // Flag to quit the Tick() look
            GfxConfiguration m_Config;
    };
}
