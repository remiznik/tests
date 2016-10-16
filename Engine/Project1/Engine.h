#ifndef _ENGINE_H
#define _ENGINE_H

#include "context.h"

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#ifndef _MAP_
#include <map>
#endif

enum SystemType;

class System;
class Game;

enum class EngineState
{
	Invalid,
	Constructing, 
	Initializing,
	Running,
	ShuttingDown,
	Destroying
};

class Engine
{
public:
	Engine();
	~Engine();

	int RunLoop();

	static EngineState GetEngineState() { return m_EngineState; }

private:
	int Initialize();
	int Draw(const Context& context);
	int Update(const Context& context);
	int ShutDown();

	int AddSystem(System* system);

	template<typename T>
	T* GetSystem(SystemType systype)
	{
		T* system = static_cast<T*>(m_mapSystems[systype]);
		if (!system)
		{
			return nullptr;
		}
		return system;
	}

	Game* CreateGame();

	std::map<SystemType, System*> m_mapSystems;
	static EngineState m_EngineState;
};

#endif

