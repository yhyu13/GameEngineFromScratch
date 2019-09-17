#include <stdio.h>
#include "EmptyApplication.hpp"
#include "Scheduler.hpp"

using namespace My;
namespace My {
    extern IApplication* g_pApp;
    extern MemoryManager*   g_pMemoryManager;
    extern GraphicsManager* g_pGraphicsManager;
    extern AssetLoader* g_pAssetLoader;
    extern SceneManager* g_pSceneManager;
}

template< class Function, class... Args>
int initiate( Function&& f, Args&&... args )
{
    try
	{
        f(args...);
    }
    catch( const EngineException& e )
	{
        BaseApplication::SetQuit(true);
		const std::wstring eMsg = e.GetFullMessage() +
			L"\n\nException caught at main window creation.";
        std::wcerr << e.GetExceptionType().c_str() << std::endl;
        std::wcerr << eMsg.c_str() << std::endl;
	}
	catch( const std::exception& e )
	{
        BaseApplication::SetQuit(true);
		// need to convert std::exception what() string from narrow to wide string
		const std::string whatStr( e.what() );
		const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) +
			L"\n\nException caught at main window creation.";
        std::wcerr << "Unhandled STL Exception" << std::endl;
        std::wcerr << eMsg.c_str() << std::endl;
	}
	// catch( ... )
	// {
    //     BaseApplication::SetQuit(true);
    //     std::wcerr << L"Unhandled Non-STL Exception" << std::endl;
    //     std::wcerr << L"Exception caught at main window creation." << std::endl;
	// }
    return 0;
}

template< class Function, class... Args>
int FixedUpdate(double waitTime, Function&& f, Args&&... args )
{
    Timer drawTimer;
    try
    {
        while (!g_pApp->IsQuit())
        {
            drawTimer.Reset();
            f(args...);
            while(drawTimer.Mark() < waitTime) {};
        }
    }
    catch(const EngineException & e)
    {
        BaseApplication::SetQuit(true);
        const std::wstring eMsg = e.GetFullMessage() + 
            L"\n\nException caught at Windows message loop.";
        BaseApplication* base = dynamic_cast<BaseApplication*>(g_pApp);
        base->ShowMessage( e.GetExceptionType(),eMsg );
    }
    catch(const std::exception& e)
    {
        BaseApplication::SetQuit(true);
        // need to convert std::exception what() string from narrow to wide string
        const std::string whatStr( e.what() );
        const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) + 
            L"\n\nException caught at Windows message loop.";
        BaseApplication* base = dynamic_cast<BaseApplication*>(g_pApp);
        base->ShowMessage( L"Unhandled STL Exception",eMsg );
    }
    // catch( ... )
    // {
    //     BaseApplication::SetQuit(true);
    //     BaseApplication* base = dynamic_cast<BaseApplication*>(g_pApp);
    //     base->ShowMessage( L"Unhandled Non-STL Exception",L"Exception caught at Windows message loop." );
    // }
    return 0;
}

int main() {

    initiate(
        [] {
            int ret = 0;
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
            if ((ret = g_pAssetLoader->Initialize()) != 0) {
                printf("Asset Loader Initialize failed, will exit now.");
                return ret;
            }
            if ((ret = g_pSceneManager->Initialize()) != 0) {
                printf("Scene Manager Initialize failed, will exit now.");
                return ret;
            }
            return ret;
        }
    );

    std::future<int> tickGroup1Future = std::async(std::launch::async,
    [] {
        FixedUpdate(1.0/2000.0, 
        [] {
            g_pMemoryManager->Tick();
            g_pGraphicsManager->Tick();
            g_pAssetLoader->Tick();
            g_pSceneManager->Tick();
            }
        );
        return 0;
    }
    );

    /* Both g_pApp Tick() (i.e., Input message loop) 
     * and OnDraw() (i.e., Rendering loop),
     * which is likely to be included in Tick() after the input message loop,
     * need to iterate in the thread that creates them (for a windows application in particular).
    */
    FixedUpdate(1.0/60.0, 
        [] {
            g_pApp->Tick();
            }
    );
    
    tickGroup1Future.get();

    initiate(
        [] {
            g_pSceneManager->Finalize();
            g_pAssetLoader->Finalize();
            g_pGraphicsManager->Finalize();
            g_pMemoryManager->Finalize();
            g_pApp->Finalize();

            delete g_pSceneManager;
            delete g_pAssetLoader;
            delete g_pGraphicsManager;
            delete g_pMemoryManager;
            delete g_pApp;
            
        }
    );
    
    return 0;
}