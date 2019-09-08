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

        virtual int Initialize();
        virtual void Finalize();
        virtual void Tick();

    protected:
        virtual void OnDraw() override;
        virtual LRESULT HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) override;

    private:
        HDC   m_hDC;
        HGLRC m_RenderContext;
    };
}
