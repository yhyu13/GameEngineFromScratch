#include "BaseApplication.hpp"
#include <iostream>

using namespace My;

bool My::BaseApplication::m_bQuit = false;
int My::BaseApplication::m_nbFrames = 0;
Timer My::BaseApplication::m_timer;

My::BaseApplication::BaseApplication(GfxConfiguration& cfg)
    :m_Config(cfg)
{
}

// Parse command line, read configuration, initialize all sub modules
int My::BaseApplication::Initialize()
{
    std::wcout << m_Config;
	return 0;
}

// Finalize all sub modules and clean up all runtime temporary files.
void My::BaseApplication::Finalize()
{
    throw 42;
}

// One cycle of the main loop
void My::BaseApplication::Tick()
{
    throw 42;
}

void My::BaseApplication::OnDraw()
{
    throw 42;
}

bool My::BaseApplication::IsQuit()
{
	return m_bQuit;
}

void My::BaseApplication::ShowMessage(const std::wstring& title,const std::wstring& message) const
{
    std::wcerr << title.c_str() << std::endl;
    std::wcerr << message.c_str() << std::endl;
}

// Static function
void My::BaseApplication::SetQuit(bool quit)
{
    m_bQuit = quit;
}

// Static function
void My::BaseApplication::OutputDebugFPS()
{
    m_nbFrames++;
    if (m_timer.Mark() >= 1.0)
    {
        std::wcout << wStr(1000.0/m_nbFrames) + L" ms/frame" << std::endl;
        m_nbFrames = 0;
        m_timer.Reset();
    }
}

            