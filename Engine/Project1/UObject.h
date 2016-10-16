#ifndef _UOBJECT_H
#define _UOBJECT_H

#include "context.h"
#include "string.h"

class UObject
{
public:
	UObject();

	UObject(const std::tstring& name);
	virtual ~UObject();

	//void* operator new(size_t size);
	//void operator delete(void* pdelete);

	virtual bool Initialize() { m_IsInitialized = true; return true; }
	virtual bool PostInitialize() { m_IsPostInitialized = true; return true; }
	virtual bool LoadContent() { m_IsContentLoaded = true; return true; }
	virtual bool PostLoadContent() { m_IsPostContentLoaded = true; return true; }
	virtual bool Update(Context& context) { return true; }

	virtual bool Draw(Context& context) { return true; }
	virtual bool DrawUI(Context& context) { return true; }
	virtual bool ShutDown() { return true; }

	virtual void Reset(); 

	void SetName(const std::tstring& name) { m_Name = name; }
	const std::tstring& GetName() const { return m_Name; }

	bool IsInitialized() const { return m_IsInitialized; }
	bool IsPostInitialized() const { return m_IsPostInitialized; }
	bool IsContentLoaded() const { return m_IsContentLoaded; }
	bool IsPostContentLoaded() const { return m_IsPostContentLoaded; }

	void SetCanTick(bool canTick) { m_CanTick = canTick; }
	bool CanTick() const { return m_CanTick; }

	void SetCanDraw(bool canDraw) { m_CanDraw = canDraw; }
	bool CanDraw() const { return m_CanDraw; }

	void Destroy() { m_IsDestroyed = true; }
	bool IsDestroyed() const { return m_IsDestroyed; }

	void Activate() { m_IsActivated = true; }
	void DeActivate() { m_IsActivated = false; }
	bool IsActivated() const { return m_IsActivated; }

protected:
	bool m_IsInitialized;
	bool m_IsPostInitialized;

	bool m_IsContentLoaded;
	bool m_IsPostContentLoaded;

	std::tstring m_Name;

	static int m_objectAmount;
	int m_ID;

private:
	bool m_IsDestroyed;

	bool m_CanTick;
	bool m_CanDraw;

	bool m_IsActivated;
};

#endif