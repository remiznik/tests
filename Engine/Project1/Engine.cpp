#include "Engine.h"

#include "System.h"
#include "Game.h"
#include "string.h"



#ifndef _DELETEMACROS_H
	#include "deletemacros.h"
#endif

EngineState Engine::m_EngineState = EngineState::Invalid;

Engine::Engine()
{
	m_EngineState = EngineState::Constructing;
}


Engine::~Engine()
{
	m_EngineState = EngineState::Destroying;
}

int Engine::RunLoop()
{
	srand(GetTickCount());

	if (!Initialize())
		return false;

	Context context;

	MSG msg = {};

	m_EngineState = EngineState::Running;

	while (msg.message != WM_QUIT && m_EngineState == EngineState::Running)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Update(context);
		Draw(context);
	}

	ShutDown();

	return msg.wParam;
}

int Engine::Initialize()
{
	m_EngineState = EngineState::Initializing;

	Game* game = CreateGame();

	if (!game)
		return false;

	return true;
}

int Engine::Draw(const Context& context)
{
	return 0;
}

int Engine::Update(const Context& context)
{
	return 0;
}

int Engine::ShutDown()
{
	m_EngineState = EngineState::ShuttingDown;

	for (std::pair<SystemType, System*> psys : m_mapSystems)
	{
		//if (!psys.second->ShutDown())
		//{
		//	continue;
		//}
		SafeDelete(psys.second);
	}
	return 0;
}

int Engine::AddSystem(System * system)
{
	auto element = m_mapSystems.insert(std::make_pair(system->GetType(), system));
	if (element.second)
		return true;
	return 0;
}

Game* Engine::CreateGame()
{
	if (!AddSystem(new Game(GameData())))
		return nullptr;

	Game* game = GetSystem<Game>(SystemType::Sys_Game);
	if (!game)
		return nullptr;

//	if (!game->Initialize())
//		return nullptr;

	return game;

}
