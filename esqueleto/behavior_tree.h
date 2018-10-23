#pragma once

class Character;
class Group;

class BehaviorTree {
public:
	BehaviorTree(Character* owner) : m_owner(owner) {}
	Character* getCharacter() const { return m_owner; }
	
	void load();
	void update(float step);

private:
	Character* m_owner;
	Group* m_rootBehavior;
};