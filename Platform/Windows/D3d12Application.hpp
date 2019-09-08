#pragma once
#include "WindowsApplication.hpp"

namespace My {
    class D3d12Application : public WindowsApplication {
    public:
        D3d12Application() = delete;
        D3d12Application( const D3d12Application& ) = delete;
        D3d12Application& operator=( const D3d12Application& ) = delete;

        D3d12Application(GfxConfiguration& config)
            : WindowsApplication(config) {};

    };
}
