#pragma once

class BehaviorTree;

enum Status
{
	eInvalid,
	eSuccess,
	eFail,
	eRunning,
};

class Behavior {
public:
	Behavior(BehaviorTree* owner) : m_owner(owner), m_Status(eInvalid) {}
	virtual Status tick(float step);

protected:
	virtual Status update(float step) = 0;
	virtual void onEnter() {}
	virtual void onExit()  {}

	BehaviorTree* m_owner;

private:
	Status m_Status;
};