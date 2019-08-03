// include the basic windows header file
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

// Screen resolution
const uint32_t SCREEN_WIDTH = 960;
const uint32_t SCREEN_HEIGHT = 480;

// Global pointers
IDXGISwapChain          *g_pSwapChain = nullptr;
ID3D11Device            *g_pDevice = nullptr;
ID3D11DeviceContext     *g_pDeviceContext = nullptr;
ID3D11RenderTargetView  *g_pRenderTargetView = nullptr;
ID3D11InputLayout       *g_pInputLayout = nullptr;
ID3D11VertexShader      *g_pVertexShader = nullptr;
ID3D11PixelShader       *g_pPixelShader = nullptr;
ID3D11Buffer            *g_pVertexBuffer = nullptr;

// Vertex Buffer Structure
struct VERTEX {
    XMFLOAT3 Position;
    XMFLOAT4 Color;
};

template<class T>
inline void SafeRelease(T **ppInterfaceToRelease)
{
    if ((*ppInterfaceToRelease) != nullptr)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = nullptr;
    }
}

void CreateRenderTarget()
{
    HRESULT hr;
    ID3D11Texture2D *pBackBuffer;
    // Get a pointer to the back buffer
    g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    // Create a render target view
    g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();
    // Bind the view
    g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
}

void SetViewPort()
{
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(viewport)); // Put viewport on the heap
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;
    g_pDeviceContext->RSSetViewports(1, &viewport);
}

// Loads the prepares shaders
void InitPipeline()
{
    // Load and compile the two shaders
    ID3DBlob *VS, *PS;
    D3DReadFileToBlob(L"copy.vso", &VS);
    D3DReadFileToBlob(L"copy.pso", &PS);
    // Encapsulate both shaders into shader objects
    g_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &g_pVertexShader);
    g_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &g_pPixelShader);
    // Set the shader objects
    g_pDeviceContext->VSSetShader(g_pVertexShader,nullptr, 0);
    g_pDeviceContext->PSSetShader(g_pPixelShader,nullptr, 0);
    // Create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] = 
    {
        {
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
        },
        {
            "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0
        },
    };
    g_pDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &g_pInputLayout);
    g_pDeviceContext->IASetInputLayout(g_pInputLayout);
    VS->Release();
    PS->Release();
}

// This is the function that creates the shape of render
void InitGraphics()
{
    // Create a triangle using the VERTEX struct
    VERTEX OurVertices[] = 
    {
        {XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
        {XMFLOAT3(0.45f, -0.5, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        {XMFLOAT3(-0.45f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f,2.0f)}
    };
    // Create the vertex bufffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC; // Write access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 3; // Size is the VERTEX struct * 3 (Drawing a triangle)
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow CPU to write in buffer

    g_pDevice->CreateBuffer(&bd, nullptr, &g_pVertexBuffer); // Create the buffer

    // Copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    // Map the Buffer
    g_pDeviceContext->Map(g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); 
    memcpy(ms.pData, OurVertices, sizeof(VERTEX) * 3); // Copy the vertercies
    g_pDeviceContext->Unmap(g_pVertexBuffer, NULL); // Unmap the buffer
}

HRESULT CreateGraphicsResources(HWND hWnd)
{
    HRESULT hr = S_OK;
    if (g_pSwapChain == nullptr)
    {
        // Create a struct to hold information about the swap chain
        DXGI_SWAP_CHAIN_DESC scd;
        ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

        // Fill the swap chain description struct
        scd.BufferCount = 1;
        scd.BufferDesc.Width = SCREEN_WIDTH;
        scd.BufferDesc.Height = SCREEN_HEIGHT;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Use 32 bits color
        scd.BufferDesc.RefreshRate.Numerator = 60;
        scd.BufferDesc.RefreshRate.Denominator = 1;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // How swap chain is to be used
        scd.OutputWindow = hWnd;                            // The Window to be used
        scd.SampleDesc.Count = 4;                           // How many multisample
        scd.Windowed = true;                                // Start as windowed
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  // Allow full-screen to windowed switching an vice versa

        const D3D_FEATURE_LEVEL FeatureLevels[] = 
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
        };
        D3D_FEATURE_LEVEL FeatureLevelSupported;

        // Create a device, device context and swap chain using the information in the scd struct
        hr = D3D11CreateDeviceAndSwapChain(
            NULL,
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            0,
            FeatureLevels,
            _countof(FeatureLevels),
            D3D11_SDK_VERSION,
            &scd,
            &g_pSwapChain,
            &g_pDevice,
            &FeatureLevelSupported,
            &g_pDeviceContext
            );
        if (hr == E_INVALIDARG)
        {
            hr = D3D11CreateDeviceAndSwapChain(
                NULL,
                D3D_DRIVER_TYPE_HARDWARE,
                NULL,
                0,
                &FeatureLevelSupported,
                1,
                D3D11_SDK_VERSION,
                &scd,
                &g_pSwapChain,
                &g_pDevice,
                nullptr,
                &g_pDeviceContext
                );
        }
        if (hr == S_OK)
        {
            CreateRenderTarget();
            SetViewPort();
            InitPipeline();
            InitGraphics();
        }
    }
    return hr;
}

void DiscardGraphicsResource()
{
    SafeRelease(&g_pSwapChain);
    SafeRelease(&g_pDevice);
    SafeRelease(&g_pDeviceContext);
    SafeRelease(&g_pRenderTargetView);
    SafeRelease(&g_pInputLayout);
    SafeRelease(&g_pVertexShader);
    SafeRelease(&g_pPixelShader);
    SafeRelease(&g_pVertexBuffer);
}

void RenderFrame()
{
    // Clear the back buffer to a deep blue
    const FLOAT clearColor[] = {.0f, .2f, .4f, 1.0f};
    g_pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, clearColor);

    // Do 3D rendering on the back buffer here
    {
        // Select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        g_pDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
        // Select which primitive type we are using
        g_pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        // Draw the vertex buffer to the back buffer
        g_pDeviceContext->Draw(3,0);
    }

    // Swap the back buffer and the front buffer
    g_pSwapChain->Present(0,0);
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
                          _T("Hello, Engine![D3D11]"),   // title of the window
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
            wasHandled = true;
            result = 0;
        } break;
        case WM_PAINT:
        {
            result = (LRESULT) CreateGraphicsResources(hWnd);
            RenderFrame();
            wasHandled = true;
        } break;
        case WM_SIZE:
        {
            if (g_pSwapChain != nullptr)
            {
                DiscardGraphicsResource();
            }
            wasHandled = true;
        } break;

        // this message is read when the window is closed
        case WM_DESTROY:
        {
            DiscardGraphicsResource();
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