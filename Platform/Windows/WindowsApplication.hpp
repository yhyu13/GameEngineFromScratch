
#define UNICODE 1
#include <windows.h>
#include <windowsx.h>
#include "BaseApplication.hpp"

namespace My {
    class WindowsApplication : public BaseApplication
    {
    public:
        // Custom exception class for windows application
        class Exception : public EngineException
        {
        public:
            using EngineException::EngineException;
            virtual std::wstring GetFullMessage() const override { return GetNote() + L"\nAt: " + GetLocation(); }
            virtual std::wstring GetExceptionType() const override { return L"Windows Exception"; }
        };
    public:
        WindowsApplication() = delete;
        WindowsApplication( const WindowsApplication& ) = delete;
        WindowsApplication& operator=( const WindowsApplication& ) = delete;

        explicit WindowsApplication(GfxConfiguration& config)
            : 
            BaseApplication(config) 
        {}

        virtual int Initialize() override;
		virtual void Finalize() override;
		// One cycle of the main loop
		virtual void Tick() override;
        virtual void ShowMessage( const std::wstring& title,const std::wstring& message ) const override;
    
    public:
        bool IsActive() const;
        bool IsMinimize() const;
        void Kill() noexcept
        {
            PostQuitMessage( 0 );
            BaseApplication::m_bQuit = true;
        }
        inline HWND GetMainWindow() { return hWnd; };

    protected:
        HWND hWnd = nullptr;
        static LPCWSTR wndClassName;
        HINSTANCE hInst = nullptr;

    protected:
        static LRESULT WINAPI _HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam );
        static LRESULT WINAPI _HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam );
        LRESULT HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam );
	    
    };
}
