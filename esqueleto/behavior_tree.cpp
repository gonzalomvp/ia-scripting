#include <stdafx.h>
#include "behavior_tree.h"
#include "behavior.h"
#include "selector.h"
#include "sequence.h"
#include "change_sprite.h"
#include "is_close.h"
#include "pursue.h"
#include "idle.h"
#include "attack.h"
#include "check_hit.h"
#include "hit.h"
#include "is_dead.h"
#include "death.h"

#include <tinyxml.h>

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
	m_rootBehavior = createBehavior(rootElem);
}

void BehaviorTree::update(float step)
{
	m_rootBehavior->tick(step);
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
			behavior = new ChangeSprite(this, std::stof(params[0]));
		}
		else if (type == "isDead") {
			behavior = new IsDead(this);
		}
		else if (type == "death") {
			behavior = new Death(this);
		}
		else if (type == "isHit") {
			behavior = new CheckHit(this);
		}
		else if (type == "hit") {
			behavior = new Hit(this);
		}
		else if (type == "distanceLessThan") {
			behavior = new IsClose(this, std::stof(params[0]));
		}
		else if (type == "attack") {
			behavior = new Attack(this);
		}
		else if (type == "chase") {
			behavior = new Pursue(this, std::stof(params[0]));
		}
		else if (type == "idle") {
			behavior = new Idle(this);
		}
	}
	return behavior;
}