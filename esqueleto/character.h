#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <moaicore/MOAIEntity2D.h>
#include <params.h>

class BaseSteering;
class StateMachine;
class BehaviorTree;

class Character: public MOAIEntity2D
{
public:
    DECL_LUA_FACTORY(Character)
protected:
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnUpdate(float step);
public:
	virtual void DrawDebug();

	Character();
	~Character();
	
	void SetLinearVelocity(float x, float y) { mLinearVelocity.mX = x; mLinearVelocity.mY = y;}
	void SetAngularVelocity(float angle) { mAngularVelocity = angle;}
	
	USVec2D GetLinearVelocity() const { return mLinearVelocity;}
	float GetAngularVelocity() const { return mAngularVelocity;}
	USVec2D GetEnemyPosition() const { return mEnemyPosition; }
	float GetEnemySpeed() const { return mEnemySpeed; }

	void AddSteering(BaseSteering* steering) { mSteerings.push_back(steering); }
	void RemoveSteering(BaseSteering* steering) { auto it = std::find(mSteerings.begin(), mSteerings.end(), steering); if (it != mSteerings.end()) mSteerings.erase(it); }
	void ClearAllSteering() { mSteerings.clear(); }
	const Params& GetParams() const { return mParams; }

protected:
	USVec2D mLinearVelocity;
	float mAngularVelocity;
	
	Params mParams;
	std::vector<USVec2D> mPath;
	std::vector<BaseSteering*> mSteerings;

	StateMachine* m_stateMachine;
	BehaviorTree* m_behaviorTree;

private:
	USVec2D mEnemyPosition;
	float mEnemySpeed;
	
	// Lua configuration
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setLinearVel(lua_State* L);
	static int _setAngularVel(lua_State* L);
};

#endif