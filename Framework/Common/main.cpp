#include <stdio.h>
#include "IApplication.hpp"
#include "BaseApplication.hpp"

using namespace My;
namespace My {
    extern IApplication* g_pApp;
}

int main() {
    int ret;
    if ((ret = g_pApp->Initialize()) != 0) {
        printf("App Initialize failed, exit now!");
        return ret;
    }
    while (!g_pApp->IsQuit()) {
        g_pApp->Tick();
    }
    g_pApp->Finalize();
    return 0;
}