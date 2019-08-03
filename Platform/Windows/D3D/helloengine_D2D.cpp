// include the basic windows header file
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <d2d1.h>

ID2D1Factory            *pFactory = nullptr;
ID2D1HwndRenderTarget   *pRenderTarget = nullptr;
ID2D1SolidColorBrush    *pLightSlateGrayBrush = nullptr;
ID2D1SolidColorBrush    *pCornflowerBlueBrush = nullptr;

template<class T>
inline void SafeRelease(T **ppInterfaceToRelease)
{
    if ((*ppInterfaceToRelease) != nullptr)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = nullptr;
    }
}

bool CreateD2DFactory()
{
    return SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory));
}

void DestoryD2DFactory()
{
    if (pFactory)
        pFactory->Release();
    pFactory = nullptr;
}

HRESULT CreateGraphicsResources(HWND hWnd)
{
    HRESULT hr = S_OK;
    if (pRenderTarget == nullptr)
    {
        RECT rc;
        GetClientRect(hWnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hWnd, size),
            &pRenderTarget
        );
        if (SUCCEEDED(hr))
        {
            hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray), &pLightSlateGrayBrush);
        }

        if (SUCCEEDED(hr))
        {
            hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &pCornflowerBlueBrush);
        }
    }
    return hr;
}

void DiscardGraphicsResource()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pLightSlateGrayBrush);
    SafeRelease(&pCornflowerBlueBrush);
}

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR lpCmdLine,
                   int nCmdShow)
{
    // initialize COM
    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) return -1;

    // the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    WNDCLASSEX wc;
    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = _T("WindowClass1");

    // register the window class
    int width = 800;
    int height = 600;
    RegisterClassEx(&wc);
    RECT wr;
	wr.left = 0;
	wr.right = width + wr.left;
	wr.top = 0;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
    int true_width = wr.right - wr.left;
	int true_height = wr.bottom - wr.top;

    RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	int horizontal = desktop.right;
	int vertical = desktop.bottom;

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(0,
                          _T("WindowClass1"),    // name of the window class
                          _T("Hello, Engine![D2D]"),   // title of the window
                          WS_OVERLAPPEDWINDOW,    // window style
                          (horizontal - width) / 2, (vertical - height) / 2, // Center of the desktop
		                  true_width, true_height,  // Window width & height including sidebars and widgets
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL

    // display the window on the screen
    ShowWindow(hWnd, nCmdShow);

    // enter the main loop:

    // this struct holds Windows event messages
    MSG msg;

    // wait for the next message in the queue, store the result in 'msg'
    while(GetMessage(&msg, NULL, 0, 0))
    {
        // translate keystroke messages into the right format
        TranslateMessage(&msg);
        // send the message to the WindowProc function
        DispatchMessage(&msg);
    }

    // Uninitialize COM
    CoUninitialize();

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    LRESULT result = 0;
    bool wasHandled = false;

    // sort through and find what code to run for the message given
    switch(message)
    {

        case WM_CREATE:
        {
            if(!CreateD2DFactory())
            {
                result = -1;
                return result;
            }
            else
            {
                wasHandled = true;
                result = 0;
            }
        } break;
        case WM_PAINT:
        {
            // PAINTSTRUCT ps;
            // HDC hdc = BeginPaint(hWnd, &ps);
            // RECT rec = {20, 20, 60, 80};
            // HBRUSH brush = (HBRUSH) GetStockObject(BLACK_BRUSH);
            // FillRect(hdc, &rec, brush);

            // EndPaint(hWnd, &ps);
            HRESULT hr = CreateGraphicsResources(hWnd);
            if (SUCCEEDED(hr))
            {
                PAINTSTRUCT ps;
                BeginPaint(hWnd, &ps);
                // Start build GPU draw command
                pRenderTarget->BeginDraw();
                // Clear the background with white color
                pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
            
                // retrieve the size of drawing area
                D2D1_SIZE_F rtSize = pRenderTarget->GetSize();
                // draw a grid background
                int width = static_cast<int>(rtSize.width);
                int height = static_cast<int>(rtSize.height);

                for (int x = 0; x < width; x += 10)
                {
                    pRenderTarget->DrawLine(
                        D2D1::Point2F(static_cast<FLOAT>(x),.0f),
                        D2D1::Point2F(static_cast<FLOAT>(x),rtSize.height),
                        pLightSlateGrayBrush,
                        .5f
                    );
                }
                for (int y = 0; y < height; y += 10)
                {
                    pRenderTarget->DrawLine(
                        D2D1::Point2F(.0f, static_cast<FLOAT>(y)),
                        D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
                        pLightSlateGrayBrush,
                        .5f
                    );
                }
                // draw two rectangles
                D2D1_RECT_F rectangle1 = D2D1::RectF(
                    rtSize.width/2 - 50.0f,
                    rtSize.height/2 - 50.0f,
                    rtSize.width/2 + 50.0f,
                    rtSize.height/2 + 50.0f
                );
                pRenderTarget->FillRectangle(&rectangle1, pLightSlateGrayBrush);
                D2D1_RECT_F rectangle2 = D2D1::RectF(
                    rtSize.width/2 - 100.0f,
                    rtSize.height/2 - 100.0f,
                    rtSize.width/2 + 100.0f,
                    rtSize.height/2 + 100.0f
                );
                pRenderTarget->DrawRectangle(&rectangle2, pCornflowerBlueBrush);

                // End GPU draw command building
                hr = pRenderTarget->EndDraw();
                if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
                {
                    DiscardGraphicsResource();
                }
                EndPaint(hWnd, &ps);
            }
            wasHandled = true;
        } break;
        case WM_SIZE:
        {
            if (pRenderTarget != nullptr)
            {
                RECT rc;
                GetClientRect(hWnd, &rc);
                D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
                pRenderTarget->Resize(size);
            }
            wasHandled = true;
        } break;

        // this message is read when the window is closed
        case WM_DESTROY:
        {
            DiscardGraphicsResource();
            DestoryD2DFactory();
            // close the application entirely
            PostQuitMessage(0);
            result = 0;
            wasHandled = true;
        } break;

        case WM_DISPLAYCHANGE:
        {
            InvalidateRect(hWnd, nullptr, false);
            wasHandled = true;
        } break;
    }

    // Handle any messages the switch statement didn't
    return (wasHandled)? result : DefWindowProc (hWnd, message, wParam, lParam);
}