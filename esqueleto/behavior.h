#pragma once

enum Status
{
	eInvalid,
	eSuccess,
	eFail,
	eRunning,
};

class Behavior {
public:
	Behavior();
	virtual Status tick();

protected:
	virtual Status update() {}
	virtual void onEnter() {}
	virtual void onExit() {}

private:
	Status m_Status;
};