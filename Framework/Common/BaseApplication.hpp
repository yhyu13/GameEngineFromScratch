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
            
            virtual int Initialize() override;
            virtual void Finalize() override;
            virtual void Tick() override;
            virtual bool IsQuit() override;
            virtual GfxConfiguration& GetConfiguration() override
            {
                return m_Config;
            }

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
