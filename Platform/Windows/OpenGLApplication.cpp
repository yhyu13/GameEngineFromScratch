#include <stdio.h>
#include <tchar.h>
#include "OpenGLApplication.hpp"
#include "OpenGL/OpenGLGraphicsManager.hpp"
#include "MemoryManager.hpp"
#include "AssetLoader.hpp"
#include "SceneManager.hpp"
#include "glad/glad_wgl.h"

using namespace My;

namespace My {
    GfxConfiguration config(8, 8, 8, 8, 32, 0, 0, 1280, 720, L"Game Engine From Scratch (Windows-OpenGL)");
	IApplication* g_pApp                = static_cast<IApplication*>(new OpenGLApplication(config));
    GraphicsManager* g_pGraphicsManager = static_cast<GraphicsManager*>(new OpenGLGraphicsManager);
    MemoryManager*   g_pMemoryManager   = static_cast<MemoryManager*>(new MemoryManager);
    AssetLoader*     g_pAssetLoader     = static_cast<AssetLoader*>(new AssetLoader);
    SceneManager*    g_pSceneManager    = static_cast<SceneManager*>(new SceneManager);
}

int My::OpenGLApplication::Initialize()
{
    int result;
    result = WindowsApplication::Initialize();
    if (result) {
        printf("Windows Application initialize failed!");
    } else {
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion   = 1;
        pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Opengl only supports single/double buffering
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = m_Config.redBits + m_Config.greenBits + m_Config.blueBits + m_Config.alphaBits;
        pfd.cDepthBits = m_Config.depthBits;
        pfd.iLayerType = PFD_MAIN_PLANE;

        m_hDC  = GetDC(hWnd);
        // Set a temporary default pixel format.
        int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
        if (nPixelFormat == 0) return -1;

        result = SetPixelFormat(m_hDC, nPixelFormat, &pfd);
        if(result != 1)
        {
                return -1;
        }

        // Create a temporary rendering context.
        m_RenderContext = wglCreateContext(m_hDC);
        if(!m_RenderContext)
        {
                return -1;
        }

        // Set the temporary rendering context as the current rendering context for this window.
        result = wglMakeCurrent(m_hDC, m_RenderContext);
        if(result != 1)
        {
                return -1;
        }

        if (!gladLoadWGL(m_hDC)) {
            printf("WGL initialize failed!\n");
            result = -1;
        } else {
            result = 0;
            printf("WGL initialize finished!\n");
        }
    }

    return result;
}

void My::OpenGLApplication::Finalize()
{
    if (m_RenderContext) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_RenderContext);
        m_RenderContext = 0;
    }
    ReleaseDC(hWnd, m_hDC);
    WindowsApplication::Finalize();
}

void My::OpenGLApplication::Tick()
{
    My::WindowsApplication::Tick();
}

void My::OpenGLApplication::OnDraw()
{
    My::WindowsApplication::OnDraw();
    g_pGraphicsManager->Clear();
    g_pGraphicsManager->Draw();
    // Opengl automatically manages v-sync as along as wglSwapIntervalEXT(1)
    SwapBuffers(m_hDC);
}

LRESULT My::OpenGLApplication::HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
    // sort through and find what code to run for the message given
    switch(msg)
    {
        case WM_LBUTTONDOWN:
        {
            const POINTS pt = MAKEPOINTS( lParam );
            throw WindowsException(_CRT_WIDE(__FILE__), __LINE__, L"Test");
        } 
        break;

         // this message is read when the window is closed
        case WM_DESTROY:
        {
            // close the application entirely
            // Since the function to accept message must be a static function
            // Calling a member function like Kill() is not allowed.
            // This is the reason to use _HandleMsgSetup initially and
            // store the window we created on Win32 side. Then retrive the window
            // pointer and make HandleMsg member function call in order to make calling
            // member function possible.
            this->Kill();
        } 
        break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, msg, wParam, lParam);
}
