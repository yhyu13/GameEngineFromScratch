#include <stdio.h>
#include "IApplication.hpp"
#include "BaseApplication.hpp"
#include "EngineException.hpp"

using namespace My;
namespace My {
    extern IApplication* g_pApp;
}

int main() {

    try
	{
        int ret;
        if ((ret = g_pApp->Initialize()) != 0) 
        {
            printf("App Initialize failed, exit now!");
            return ret;
        }

        try
        {
            while (!g_pApp->IsQuit()) 
            {
                g_pApp->Tick();
            }
            g_pApp->Finalize();
            return 0;
        }
        catch(const EngineException & e)
        {
            const std::wstring eMsg = e.GetFullMessage() + 
				L"\n\nException caught at Windows message loop.";
            if(BaseApplication* base = dynamic_cast<BaseApplication*>(g_pApp))
            {
                base->ShowMessage( e.GetExceptionType(),eMsg );
            }
            else
            {
                std::cerr << e.GetExceptionType().c_str() << std::endl;
                std::cerr << eMsg.c_str() << std::endl;
            }
        }
        catch(const std::exception& e)
        {
            // need to convert std::exception what() string from narrow to wide string
			const std::string whatStr( e.what() );
			const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) + 
				L"\n\nException caught at Windows message loop.";
            if(BaseApplication* base = dynamic_cast<BaseApplication*>(g_pApp))
            {
                base->ShowMessage( L"Unhandled STL Exception",eMsg );
            }
            else
            {
                std::cerr << "Unhandled STL Exception" << std::endl;
                std::cerr << eMsg.c_str() << std::endl;
            }
        }
        catch( ... )
		{
			if(BaseApplication* base = dynamic_cast<BaseApplication*>(g_pApp))
            {
                base->ShowMessage( L"Unhandled Non-STL Exception",L"\n\nException caught at Windows message loop." );
            }
            else
            {
                std::cerr << "Unhandled Non-STL Exception" << std::endl;
                std::cerr << "Exception caught at Windows message loop." << std::endl;
            }
		}
    }
    catch( const EngineException& e )
	{
		const std::wstring eMsg = e.GetFullMessage() +
			L"\n\nException caught at main window creation.";
        std::cerr << e.GetExceptionType().c_str() << std::endl;
        std::cerr << eMsg.c_str() << std::endl;
	}
	catch( const std::exception& e )
	{
		// need to convert std::exception what() string from narrow to wide string
		const std::string whatStr( e.what() );
		const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) +
			L"\n\nException caught at main window creation.";
        std::cerr << "Unhandled STL Exception" << std::endl;
        std::cerr << eMsg.c_str() << std::endl;
	}
	catch( ... )
	{
        std::cerr << "Unhandled Non-STL Exception" << std::endl;
        std::cerr << "Exception caught at main window creation." << std::endl;
	}

	return 0;
}