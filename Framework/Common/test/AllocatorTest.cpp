#include <iostream>
#include <string>
#include <vector>
#include "MemoryManager.hpp"
#include "Utility.hpp"
#include "Timer.hpp"

#include "geommath.hpp"

using namespace std;
using namespace My;

namespace My {
    MemoryManager* g_pMemoryManager = new MemoryManager();
}

int main(int , char** )
{
    g_pMemoryManager->Initialize();

    auto timer = Timer();
    for (int i = 0; i < 10000; i++)
    {
        int* a = g_pMemoryManager->New<int>(30);
        g_pMemoryManager->Delete(a);
    }
    wcout << L"MyMemoryManager Score (int): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        int* a = new int(30);
        delete a;
    }
    wcout << L"C++NativeMalloc Score (int): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        char* space = (char*)g_pMemoryManager->Allocate(1024);
        g_pMemoryManager->Delete(space);
    }
    wcout << L"MyMemoryManager Score (1kb): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        char* space = (char*)malloc(1024);
        free(space);
    }
    wcout << L"C++NativeMalloc Score (1kb): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        char* space = (char*)g_pMemoryManager->Allocate(4096);
        g_pMemoryManager->Delete(space);
    }
    wcout << L"MyMemoryManager Score (4kb): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        char* space = (char*)malloc(4096);
        free(space);
    }
    wcout << L"C++NativeMalloc Score (4kb): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        Vector4f* vec = g_pMemoryManager->New<Vector4f>();
        g_pMemoryManager->Delete(vec);
    }
    wcout << L"MyMemoryManager Score (Vector4f): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        Vector4f* vec = new Vector4f();
        delete vec;
    }
    wcout << L"C++NativeMalloc Score (Vector4f): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        std::vector<int, My::allocator<int>> vec2;
        vec2.push_back(1);
    }
    wcout << L"MyMemoryManager Score (vec int): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        std::vector<int> vec2;
        vec2.push_back(1);
    }
    wcout << L"C++NativeMalloc Score (vec int): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        std::vector<Vector4f, My::allocator<Vector4f>> vec2;
        vec2.push_back(Vector4f());
    }
    wcout << L"MyMemoryManager Score (vec Vector4f): " << wStr(timer.Mark()) << endl;

    timer.Reset();
    for (int i = 0; i < 10000; i++)
    {
        std::vector<Vector4f> vec2;
        vec2.push_back(Vector4f());
    }
    wcout << L"C++NativeMalloc Score (vec Vector4f): " << wStr(timer.Mark()) << endl;

    char* space1 = (char*)g_pMemoryManager->Allocate(11);
    char* temp = space1;
    for (int j  = 0; j < 10; j++)
    {
        *temp = ('0'+j);
        temp++;
    }
    *temp = '\0';
    wcout<< space1 << endl;
    g_pMemoryManager->Delete(space1);

    g_pMemoryManager->Finalize();

    delete g_pMemoryManager;

    return 0;
}

