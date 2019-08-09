// include the basic windows header file
#include "WindowsApplication.hpp"
#include <tchar.h>

 using namespace My;

 // the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam);

 namespace My {
    GfxConfiguration config(8, 8, 8, 8, 32, 0, 0, 960, 540, L"Game Engine From Scratch (Windows)");
    WindowsApplication  g_App(config);
    IApplication*       g_pApp = &g_App;
}

 int My::WindowsApplication::Initialize()
{
    //https://docs.microsoft.com/en-us/windows/win32/api/windef/ne-windef-dpi_awareness
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

    int result;

     result = BaseApplication::Initialize();

     if (result != 0)
        exit(result);

     // get the HINSTANCE of the Console Program
    HINSTANCE hInstance = GetModuleHandle(NULL);

     // the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    WNDCLASSEX wc;

     // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

     // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = _T("GameEngineFromScratch");
     // register the window class
    RegisterClassEx(&wc);

    DWORD WindowStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
    RECT desktop;
    RECT wnd = {0,0,m_Config.screenWidth,m_Config.screenHeight};
    GetClientRect(GetDesktopWindow(), &desktop);
    AdjustWindowRect(&wnd, WindowStyle, FALSE);
    float DPI_Scale = 1.0f;

    int w = (int)((wnd.right - wnd.left) * DPI_Scale);
    int h = (int)((wnd.bottom - wnd.top) * DPI_Scale);
    int x = (int)(((desktop.right - desktop.left) - w) / 2) ;
    int y = (int)(((desktop.bottom - desktop.top) - h) / 2 );

     // create the window and use the result as the handle
    hWnd = CreateWindowExW(0,
                          L"GameEngineFromScratch",      // name of the window class
                          m_Config.appName,             // title of the window
                          WindowStyle,              // window style
                          x,//CW_USEDEFAULT,                    // x-position of the window
                          y,//CW_USEDEFAULT,                    // y-position of the window
                          w,             // width of the window
                          h,            // height of the window
                          NULL,                             // we have no parent window, NULL
                          NULL,                             // we aren't using menus, NULL
                          hInstance,                        // application handle
                          NULL);                            // used with multiple windows, NULL

     // display the window on the screen
    ShowWindow(hWnd, SW_SHOW);

     return result;
}

 void My::WindowsApplication::Finalize()
{
}

 void My::WindowsApplication::Tick()
{
    // this struct holds Windows event messages
    MSG msg;

     // we use PeekMessage instead of GetMessage here
    // because we should not block the thread at anywhere
    // except the engine execution driver module 
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        // translate keystroke messages into the right format
        TranslateMessage(&msg);

         // send the message to the WindowProc function
        DispatchMessage(&msg); 
    }
}

 // this is the main message handler for the program
LRESULT CALLBACK My::WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
	case WM_PAINT:
        // we will replace this part with Rendering Module
	    {
	    } break;

         // this message is read when the window is closed
    case WM_DESTROY:
        {
            // close the application entirely
            PostQuitMessage(0);
            BaseApplication::m_bQuit = true;
            return 0;
        }
    }

     // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}
