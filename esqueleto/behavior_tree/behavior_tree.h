#pragma once

class Character;
class Behavior;
class TiXmlElement;

class BehaviorTree {
public:
	BehaviorTree(Character* owner) : mOwner(owner), mRootBehavior(nullptr) {}
	~BehaviorTree();
	
	Character* getCharacter() const { return mOwner; }
	
	bool load(const char* filename);
	Behavior* createBehavior(TiXmlElement* behaviorElem);
	void update(float step);

private:
	Character* mOwner;
	Behavior*  mRootBehavior;
};