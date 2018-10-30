#pragma once

class Character;
class Group;
class Behavior;
class TiXmlElement;

class BehaviorTree {
public:
	BehaviorTree(Character* owner) : m_owner(owner) {}
	Character* getCharacter() const { return m_owner; }
	
	bool load(const char* filename);
	Behavior* createBehavior(TiXmlElement* behaviorElem);
	void update(float step);

private:
	Character* m_owner;
	Behavior* m_rootBehavior;
};