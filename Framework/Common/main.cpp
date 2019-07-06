#include <stdio.h>
#include "IApplication.hpp"
#include "BaseApplication.hpp"

using namespace My;
namespace My {
    extern IApplication* g_App;
}

int main() {
    int ret;
    if ((ret = g_App->Initialize()) != 0) {
        printf("App Initialize failed, exit now!");
        return ret;
    }
    while (!g_App->IsQuit()) {
        g_App->Tick();
    }
    g_App->Finalize();
    return 0;
}