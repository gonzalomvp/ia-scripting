#include <stdafx.h>
#include "behavior_tree.h"
#include "behavior.h"
#include "selector.h"
#include "sequence.h"
#include "behavior_tree/actions/change_sprite.h"
#include "behavior_tree/actions/hit.h"
#include "behavior_tree/actions/idle.h"
#include "behavior_tree/actions/pursue.h"
#include "behavior_tree/conditions/check_hit.h"
#include "behavior_tree/actions/death.h"
#include "behavior_tree/conditions/check_dead.h"
#include "behavior_tree/conditions/check_distance.h"

#include <tinyxml.h>

BehaviorTree::~BehaviorTree() {
	delete mRootBehavior;
}

bool BehaviorTree::load(const char* filename) {
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		fprintf(stderr, "Couldn't read behavior tree configuration from %s", filename);
		return false;
	}

	TiXmlHandle hDoc(&doc);

	TiXmlElement* pElem;
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		fprintf(stderr, "Invalid format for %s", filename);
		return false;
	}

	TiXmlHandle hRoot(pElem);

	TiXmlHandle hParams = hRoot.FirstChildElement("behavior");
	TiXmlElement* rootElem = hParams.Element();
	mRootBehavior = createBehavior(rootElem);
	return true;
}

void BehaviorTree::update(float step)
{
	mRootBehavior->tick(step);
}

Behavior* BehaviorTree::createBehavior(TiXmlElement* behaviorElem) {
	Behavior* behavior = nullptr;
	if (behaviorElem) {
		std::string type = behaviorElem->Attribute("type");
		std::vector<std::string> params;
		TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
		for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
			params.push_back(paramElem->Attribute("value"));
		}

		std::vector<Behavior*> childBehaviors;
		TiXmlElement* childElem = behaviorElem->FirstChildElement("behavior");
		for (childElem; childElem; childElem = childElem->NextSiblingElement()) {
			Behavior* childBehavior = createBehavior(childElem);
			if (childBehavior) {
				childBehaviors.push_back(childBehavior);
			}
		}

		if (type == "selector") {
			Selector* selector = new Selector(this);
			for (size_t i = 0; i < childBehaviors.size(); ++i) {
				selector->AddBehavior(childBehaviors[i]);
			}
			behavior = selector;
		}
		else if (type == "sequence") {
			Sequence* sequence = new Sequence(this);
			for (size_t i = 0; i < childBehaviors.size(); ++i) {
				sequence->AddBehavior(childBehaviors[i]);
			}
			behavior = sequence;
		}
		else if (type == "changeSprite") {
			behavior = new ChangeSprite(this, std::stoi(params[0]));
		}
		else if (type == "checkDead") {
			behavior = new CheckDead(this);
		}
		else if (type == "death") {
			behavior = new Death(this);
		}
		else if (type == "checkHit") {
			behavior = new CheckHit(this);
		}
		else if (type == "hit") {
			behavior = new Hit(this);
		}
		else if (type == "checkDistance") {
			behavior = new CheckDistance(this, std::stof(params[0]));
		}
		else if (type == "chase") {
			behavior = new Pursue(this, std::stof(params[0]), std::stof(params[1]));
		}
		else if (type == "idle") {
			behavior = new Idle(this);
		}
	}
	return behavior;
}