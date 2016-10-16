#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "UObject.h"
#include "context.h"

enum SystemType
{
	Sys_Invalid,
	Sys_Window,
	Sys_Game,
	Sys_Input,
	Sys_Graphics,
	Sys_GameTimer,

	Sys_MAX
};

struct SystemData
{
	SystemData();
	SystemData(const SystemType& type);

	SystemType systemType;
};

class System :
	public UObject
{
	friend class Engine;

public:
	SystemType GetType() const { return m_SystemType; }

protected:
	System(const SystemData& data);
	virtual ~System();

	bool Initialize() override { return UObject::Initialize(); }
	bool Update(Context& context) override { return UObject::Update(context); }
	bool ShutDown() override { return UObject::ShutDown(); }	

protected:
	SystemType m_SystemType;
};

#endif

