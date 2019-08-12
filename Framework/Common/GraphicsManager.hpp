#pragma once
#include "IRuntimeModule.hpp"
#include "EngineException.hpp"

namespace My {
	class GraphicsManager : implements IRuntimeModule
	{
	public:
		virtual ~GraphicsManager() {}

       	virtual int Initialize();
	    virtual void Finalize();

	    virtual void Tick();
	};
}

