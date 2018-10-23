#include <stdafx.h>
#include "selector.h"

Status Selector::update(float step)
{
	m_CurrentChild = 0;
	while (true)
	{
		Status s = m_Children[m_CurrentChild]->tick(step);
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