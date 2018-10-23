#include <stdafx.h>
#include "behavior_tree.h"
#include "behavior.h"
#include "selector.h"
#include "sequence.h"
#include "is_close.h"
#include "pursue.h"
#include "idle.h"
#include "attack.h"
#include "check_hit.h"
#include "hit.h"
#include "is_dead.h"
#include "death.h"

void BehaviorTree::load() {
	m_rootBehavior = new Selector(this);

	Sequence* isDeadBranch = new Sequence(this);
	isDeadBranch->AddBehavior(new IsDead(this));
	isDeadBranch->AddBehavior(new Death(this));
	
	Sequence* receiveHitBranch = new Sequence(this);
	receiveHitBranch->AddBehavior(new CheckHit(this));
	receiveHitBranch->AddBehavior(new Hit(this));

	Selector* closeBranch = new Selector(this);
	Sequence* attackBranch = new Sequence(this);
	Sequence* chaseBranch = new Sequence(this);
	closeBranch->AddBehavior(attackBranch);
	closeBranch->AddBehavior(chaseBranch);

	attackBranch->AddBehavior(new IsClose(this, 50.0f));
	attackBranch->AddBehavior(new Attack(this));

	chaseBranch->AddBehavior(new IsClose(this, 200.0f));
	chaseBranch->AddBehavior(new Pursue(this));

	m_rootBehavior->AddBehavior(isDeadBranch);
	m_rootBehavior->AddBehavior(receiveHitBranch);
	m_rootBehavior->AddBehavior(closeBranch);
	m_rootBehavior->AddBehavior(new Idle(this));


	//State* idle = new State();
	//Action* idleAction = new ChangeSpriteAction(this, 0);
	//idle->setEnterAction(idleAction);
	//m_States.push_back(idle);

	//State* alarm = new State();
	//Action* alarmAction = new ChangeSpriteAction(this, 3);
	//alarm->setEnterAction(alarmAction);
	//Action* pursueAction = new PursueAction(this);
	//alarm->setStateAction(pursueAction);
	//m_States.push_back(alarm);

	//Transition* closeTransition = new Transition(new DistanceCondition(this), alarm);
	//idle->addTransition(closeTransition);

	//Transition* farTransition = new Transition(new NotCondition(this, new DistanceCondition(this)), idle);
	//alarm->addTransition(farTransition);
}

void BehaviorTree::update(float step)
{
	m_rootBehavior->tick(step);
}