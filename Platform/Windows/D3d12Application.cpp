#include "WindowsApplication.hpp"
#include "D3d/D3d12GraphicsManager.hpp"
#pragma once
#include "MemoryManager.hpp"
#include <tchar.h>

using namespace My;

namespace My {
    GfxConfiguration config(8, 8, 8, 8, 32, 0, 0, 1920, 720, L"Game Engine From Scratch (Windows)");
	IApplication* g_pApp                = static_cast<IApplication*>(new WindowsApplication(config));
    GraphicsManager* g_pGraphicsManager = static_cast<GraphicsManager*>(new D3d12GraphicsManager);
    MemoryManager*   g_pMemoryManager   = static_cast<MemoryManager*>(new MemoryManager);
}

