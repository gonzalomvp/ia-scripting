#include <stdafx.h>
#include "behavior.h"

Status Behavior::tick(float step)
{
	if (m_Status != eRunning)
	{
		onEnter();
	}
	m_Status = update(step);
	if (m_Status != eRunning)
	{
		onExit();
	}
	return m_Status;
}