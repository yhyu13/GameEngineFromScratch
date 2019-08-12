#include <iostream>
#include <string>
#include "MemoryManager.hpp"

using namespace std;
using namespace My;

namespace My {
    MemoryManager* g_pMemoryManager = new MemoryManager();
}

int main(int , char** )
{
    g_pMemoryManager->Initialize();

    int* i = g_pMemoryManager->New<int>();
    *i = 30;
    wcout << i << endl;
    g_pMemoryManager->Delete(i);

    char* space1 = (char*)g_pMemoryManager->Allocate(1024);
    char* temp = space1;
    for (int j  = 0; j < 10; j++)
    {
        *temp = '1';
        temp++;
    }
    *temp = '\0';
    wcout<< space1 << endl;
    g_pMemoryManager->Delete(space1);

    g_pMemoryManager->Finalize();

    delete g_pMemoryManager;

    return 0;
}

