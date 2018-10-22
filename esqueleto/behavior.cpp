#include <stdafx.h>
#include "behavior.h"

Status Behavior::tick()
{
	if (m_Status == eInvalid)
	{
		onEnter();
	}
	m_Status = update();
	if (m_Status != eRunning)
	{
		onExit();
	}
	return m_Status;
}