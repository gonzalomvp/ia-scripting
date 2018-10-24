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
	
	// Getters and Setters
	USVec2D                     GetLinearVelocity()  const { return mLinearVelocity;  }
	float                       GetAngularVelocity() const { return mAngularVelocity; }
	bool                        GetIsHit()           const { return mIsHit;           }
	float                       GetLife()            const { return mLife;            }
	USVec2D                     GetEnemyPosition()   const { return mEnemyPosition;   }
	USVec2D                     GetEnemyVelocity()   const { return mEnemyVelocity;   }
	Params&                     GetParams()                { return mParams;          }
	const std::vector<USVec2D>& GetPath()            const { return mPath;            }
	const std::vector<USVec3D>& GetObstacles()       const { return mObstacles;       }

	void SetLinearVelocity  (float x, float y) { mLinearVelocity  = USVec2D(x, y); }
	void SetAngularVelocity (float angle)      { mAngularVelocity = angle;         }
	void SetIsHit           (bool value)       { mIsHit           = value;         }
	void SetLife            (float value)      { mLife            = value;         }

	//Borrar!
	void AddSteering(BaseSteering* steering) { mSteerings.push_back(steering); }
	void RemoveSteering(BaseSteering* steering) { auto it = std::find(mSteerings.begin(), mSteerings.end(), steering); if (it != mSteerings.end()) mSteerings.erase(it); }
	void ClearAllSteering() { mSteerings.clear(); }

private:
	USVec2D                    mLinearVelocity;
	float                      mAngularVelocity;
	Params                     mParams;
	bool                       mIsHit;
	float                      mLife;
	USVec2D                    mEnemyPosition;
	USVec2D                    mEnemyTarget;
	USVec2D                    mEnemyVelocity;
	std::vector<USVec2D>       mPath;
	std::vector<USVec3D>       mObstacles;
	std::vector<BaseSteering*> mSteerings;
	StateMachine*              mStateMachine;
	BehaviorTree*              mBehaviorTree;


	// Lua configuration
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setLinearVel(lua_State* L);
	static int _setAngularVel(lua_State* L);
	static int _checkHit(lua_State* L);
};

#endif