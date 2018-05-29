#include <stdafx.h>
#include "state.h"

#include "action.h"
#include "transition.h"

void State::onEnter()
{
	if (m_enterAction)
	{
		m_enterAction->start();
		m_enterAction->update();
		m_enterAction->end();
	}
	if (m_stateAction)
	{
		m_stateAction->start();
	}
}

void State::update()
{
	if (m_stateAction)
	{
		m_stateAction->update();
	}
}

void State::onExit()
{
	if (m_stateAction)
	{
		m_stateAction->end();
	}
	if (m_exitAction)
	{
		m_exitAction->start();
		m_exitAction->update();
		m_exitAction->end();
	}
}
