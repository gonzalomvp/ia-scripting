#pragma once
#include "action.h"

class ChangeSpriteAction : public Action {
public:
	ChangeSpriteAction(const char* imageFile);
	virtual void start();

private:
	char m_imageFile[256];
};