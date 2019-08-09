#pragma once
#include "IApplication.hpp"
#include "GfxConfiguration.h"

namespace My {
    class BaseApplication : implements IApplication {
        public:
            // Disable default constructor
            BaseApplication() = delete;
            explicit BaseApplication(GfxConfiguration& cfg);
            
            virtual int Initialize();
            virtual void Finalize();
            virtual void Tick();
            virtual bool IsQuit();

        protected:
            void SetIsQuit(bool bQuit) {
                m_bQuit = bQuit;
            }

        protected:
            static bool m_bQuit;          // Flag to quit the Tick() look
            GfxConfiguration m_Config;
    };
}
