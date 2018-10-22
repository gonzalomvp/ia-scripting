#include <stdafx.h>
#include "selector.h"

Status Selector::update()
{
	while (true)
	{
		Status s = m_Children[m_CurrentChild]->tick();
		if (s != eFail)
		{
			return s;
		}
		++m_CurrentChild;
		if (m_CurrentChild == m_Children.size())
		{
			return eFail;
		}
	}
	return eInvalid;
}

void Selector::onEnter()
{
	m_CurrentChild = 0;
}