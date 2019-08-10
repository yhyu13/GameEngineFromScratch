#pragma once
#include "WindowsApplication.hpp"

namespace My {
    class OpenGLApplication : public WindowsApplication {
    public:
        OpenGLApplication() = delete;
        OpenGLApplication( const OpenGLApplication& ) = delete;
        OpenGLApplication& operator=( const OpenGLApplication& ) = delete;

        OpenGLApplication(GfxConfiguration& config)
            : WindowsApplication(config) {};

        virtual int Initialize() override;
        virtual void Finalize() override;
        virtual void Tick() override;

    protected:
        HGLRC m_RenderContext;
    };
}
