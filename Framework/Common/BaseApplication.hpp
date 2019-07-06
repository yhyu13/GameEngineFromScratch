#pragma once
#include "IApplication.hpp"

namespace My {
    class BaseApplication : implements IApplication {
        public:
            virtual int Initialize();
            virtual void Finalize();
            virtual void Tick();
            virtual bool IsQuit();

        protected:
            void SetIsQuit(bool bQuit) {
                m_bQuit = bQuit;
            }


        private:
            bool m_bQuit;           // Flag to quit the Tick() look
    };
}