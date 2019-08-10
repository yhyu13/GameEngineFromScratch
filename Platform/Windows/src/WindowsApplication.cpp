// include the basic windows header file
#include "WindowsApplication.hpp"
#include <tchar.h>
#include <assert.h>

using namespace My;

namespace My {
    GfxConfiguration config(8, 8, 8, 8, 32, 0, 0, 1920, 720, L"Game Engine From Scratch (Windows)");
    WindowsApplication  g_App(config);
    IApplication*       g_pApp = &g_App;
}

LPCWSTR My::WindowsApplication::wndClassName = L"GameEngineFromScratch";

 int My::WindowsApplication::Initialize()
{
    // This api call make the process be aware of dpi 
    // i.e. managing dpi by ourself instead of Desktop Window Manager(DWM)
    // i.e. ignoring user dpi scales which are common to be 125%, 150%, etc
    // Not making this api call would result in the window we created to be scaled with 
    // user dpi setting (e.g. "Change Display Setting" in Win10).
    //https://docs.microsoft.com/en-us/windows/win32/api/windef/ne-windef-dpi_awareness
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

    int result;

     result = BaseApplication::Initialize();

     if (result != 0)
        exit(result);

     // get the HINSTANCE of the Console Program
    hInst = GetModuleHandle(NULL);

    // register window class                       // MyWindowProc // _HandleMsgSetup
	WNDCLASSEX wc = { sizeof( WNDCLASSEX ),CS_CLASSDC,_HandleMsgSetup,0,0,
		hInst,nullptr,nullptr,nullptr,nullptr,
		wndClassName,nullptr };

    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
     // register the window class
    RegisterClassEx(&wc);

    // Center and adjust the window size
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
                          wndClassName,                     // name of the window class
                          m_Config.appName,                 // title of the window
                          WindowStyle,                      // window style
                          x,                                // x-position of the window (optional: CW_USEDEFAULT)
                          y,                                // y-position of the window (optional: CW_USEDEFAULT)
                          w,                                // width of the window
                          h,                                // height of the window
                          NULL,                             // we have no parent window, NULL
                          NULL,                             // we aren't using menus, NULL
                          hInst,                        // application handle
                          this);                            // Pass "this" to get a return value during creation
                                                            // Making the last param to be NULL would fail the assertion in _HandleMsgSetup
                                                            // https://docs.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-

     // display the window on the screen
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow( hWnd );

    return result;
}

 void My::WindowsApplication::Finalize()
{
    // unregister window class
	UnregisterClass( wndClassName,hInst );
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

bool My::WindowsApplication::IsActive() const
{
	return GetActiveWindow() == hWnd;
}

bool My::WindowsApplication::IsMinimize() const
{
	return IsIconic(hWnd) != 0;
}

void My::WindowsApplication::ShowMessage( const std::wstring& title,const std::wstring& message ) const 
{
	MessageBox( hWnd,message.c_str(),title.c_str(),MB_OK );
}

LRESULT WINAPI My::WindowsApplication::_HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if( msg == WM_NCCREATE )
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		My::WindowsApplication* const pWnd = reinterpret_cast<My::WindowsApplication*>(pCreate->lpCreateParams);
		// sanity check
		assert( pWnd != nullptr );
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr( hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(pWnd) );
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr( hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&My::WindowsApplication::_HandleMsgThunk) );
		// forward message to window class handler
		return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc( hWnd,msg,wParam,lParam );
}

LRESULT WINAPI My::WindowsApplication::_HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
	// retrieve ptr to window class
	My::WindowsApplication* const pWnd = reinterpret_cast<My::WindowsApplication*>(GetWindowLongPtr( hWnd,GWLP_USERDATA ));
	// forward message to window class handler
	return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
}

LRESULT My::WindowsApplication::HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
    LRESULT result = 0;
    bool wasHandled = false;

    // sort through and find what code to run for the message given
    switch(msg)
    {
	    case WM_PAINT:
        // we will replace this part with Rendering Module
	    {
	    } 
        wasHandled = true;
        break;

         // this message is read when the window is closed
        case WM_DESTROY:
        {
            // close the application entirely
            Kill();
            return 0;
        } 
        wasHandled = true;
        break;
    }

    // Handle any messages the switch statement didn't
    if (!wasHandled) { result = DefWindowProc (hWnd, msg, wParam, lParam); }
    return result;
}

LRESULT WINAPI My::WindowsApplication::MyWindowProc( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{

    LRESULT result = 0;
    bool wasHandled = false;

    // sort through and find what code to run for the message given
    switch(msg)
    {
	    case WM_PAINT:
        // we will replace this part with Rendering Module
	    {
	    } 
        wasHandled = true;
        break;

         // this message is read when the window is closed
        case WM_DESTROY:
        {
            // close the application entirely
            {
                // Code is this scope = Kill()
                // But since MyWindowProc is a static function
                // A member function call like Kill() is not allowed.
                // This is the reason the set up _HandleMsgSetup
                PostQuitMessage( 0 );
                BaseApplication::m_bQuit = true;
            }
            return 0;
        } 
        wasHandled = true;
        break;
    }

    // Handle any messages the switch statement didn't
    if (!wasHandled) { result = DefWindowProc (hWnd, msg, wParam, lParam); }
    return result;
}

