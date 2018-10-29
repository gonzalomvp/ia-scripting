#include <stdafx.h>
#include "state.h"

#include "action.h"
#include "transition.h"

void State::onEnter(float step)
{
	if (m_enterAction)
	{
		m_enterAction->start();
		m_enterAction->update(step);
		m_enterAction->end();
	}
	if (m_stateAction)
	{
		m_stateAction->start();
	}
}

void State::update(float step)
{
	if (m_stateAction)
	{
		m_stateAction->update(step);
	}
}

void State::onExit(float step)
{
	if (m_stateAction)
	{
		m_stateAction->end();
	}
	if (m_exitAction)
	{
		m_exitAction->start();
		m_exitAction->update(step);
		m_exitAction->end();
	}
}
