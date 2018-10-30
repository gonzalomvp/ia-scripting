#include <stdafx.h>
#include "state.h"
#include "action.h"
#include "transition.h"

State::~State() {
	delete mEnterAction;
	delete mExitAction;
	delete mStateAction;

	for (size_t i = 0; i < mTransitions.size(); ++i) {
		delete mTransitions[i];
	}
}

void State::onEnter(float step) {
	if (mEnterAction) {
		mEnterAction->start();
		mEnterAction->update(step);
		mEnterAction->end();
	}
	if (mStateAction) {
		mStateAction->start();
	}
}

void State::update(float step) {
	if (mStateAction) {
		mStateAction->update(step);
	}
}

void State::onExit(float step) {
	if (mStateAction) {
		mStateAction->end();
	}
	if (mExitAction) {
		mExitAction->start();
		mExitAction->update(step);
		mExitAction->end();
	}
}
