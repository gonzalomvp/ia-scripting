#include <stdafx.h>
#include "sequence.h"

Status Sequence::update()
{
	while (true)
	{
		Status s = m_Children[m_CurrentChild]->tick();
		if (s != eSuccess)
		{
			return s;
		}
		++m_CurrentChild;

		if (m_CurrentChild == m_Children.size())
		{
			return eSuccess;
		}
	}
	return eInvalid;
}

void Sequence::onEnter()
{
	m_CurrentChild = 0;
}