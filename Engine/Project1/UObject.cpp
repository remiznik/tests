#include "UObject.h"

int UObject::m_objectAmount = 0;

UObject::UObject()
	: m_Name(_T(""))
	, m_ID(m_objectAmount)
	, m_IsInitialized(false)
	, m_IsPostInitialized(false)
	, m_IsContentLoaded(false)
	, m_IsPostContentLoaded(false)
	, m_IsDestroyed(false)
	, m_CanTick(true)
	, m_CanDraw(true)
	, m_IsActivated(true)
{
	m_Name = _T("Object_") + TOSTRING(m_ID);
	++m_objectAmount;
}

UObject::UObject(const std::tstring& name)
	: m_Name(name)
	, m_ID(m_objectAmount)
	, m_IsInitialized(false)
    , m_IsPostInitialized(false)
	, m_IsContentLoaded(false)
	, m_IsPostContentLoaded(false)
	, m_IsDestroyed(false)
	, m_CanTick(true)
	, m_CanDraw(true)
	, m_IsActivated(true)
{
	++m_objectAmount;
}


UObject::~UObject()
{
	--m_objectAmount;
}

void UObject::Reset()
{

}
