#include <stdio.h>
#include "IApplication.hpp"
#include "BaseApplication.hpp"
#include "EngineException.hpp"
#include "GraphicsManager.hpp"
#include "MemoryManager.hpp"

using namespace My;
namespace My {
    extern IApplication* g_pApp;
    extern MemoryManager*   g_pMemoryManager;
    extern GraphicsManager* g_pGraphicsManager;
}

int main() {

    try
	{
        int ret;
        if ((ret = g_pApp->Initialize()) != 0) {
            printf("App Initialize failed, will exit now.");
            return ret;
        }

        if ((ret = g_pMemoryManager->Initialize()) != 0) {
            printf("Memory Manager Initialize failed, will exit now.");
            return ret;
        }

        if ((ret = g_pGraphicsManager->Initialize()) != 0) {
            printf("Graphics Manager Initialize failed, will exit now.");
            return ret;
        }

        try
        {
            while (!g_pApp->IsQuit()) {
                g_pApp->Tick();
                g_pMemoryManager->Tick();
                g_pGraphicsManager->Tick();
            }

            g_pGraphicsManager->Finalize();
            g_pMemoryManager->Finalize();
            g_pApp->Finalize();
        }
        catch(const EngineException & e)
        {
            const std::wstring eMsg = e.GetFullMessage() + 
				L"\n\nException caught at Windows message loop.";
            BaseApplication* base = dynamic_cast<BaseApplication*>(g_pApp);
            base->ShowMessage( e.GetExceptionType(),eMsg );
        }
        catch(const std::exception& e)
        {
            // need to convert std::exception what() string from narrow to wide string
			const std::string whatStr( e.what() );
			const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) + 
				L"\n\nException caught at Windows message loop.";
            BaseApplication* base = dynamic_cast<BaseApplication*>(g_pApp);
            base->ShowMessage( L"Unhandled STL Exception",eMsg );
        }
        catch( ... )
		{
            BaseApplication* base = dynamic_cast<BaseApplication*>(g_pApp);
            base->ShowMessage( L"Unhandled Non-STL Exception",L"Exception caught at Windows message loop." );
		}
    }
    catch( const EngineException& e )
	{
		const std::wstring eMsg = e.GetFullMessage() +
			L"\n\nException caught at main window creation.";
        std::wcerr << e.GetExceptionType().c_str() << std::endl;
        std::wcerr << eMsg.c_str() << std::endl;
	}
	catch( const std::exception& e )
	{
		// need to convert std::exception what() string from narrow to wide string
		const std::string whatStr( e.what() );
		const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) +
			L"\n\nException caught at main window creation.";
        std::wcerr << "Unhandled STL Exception" << std::endl;
        std::wcerr << eMsg.c_str() << std::endl;
	}
	catch( ... )
	{
        std::wcerr << L"Unhandled Non-STL Exception" << std::endl;
        std::wcerr << L"Exception caught at main window creation." << std::endl;
	}

	return 0;
}