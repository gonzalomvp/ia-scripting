#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>
#include "params.h"
#include "character.h"

struct MapNode;

struct PathNode {
	const MapNode* mNode;
	const MapNode* mParentNode;
	float g_score;
	float f_score;
	
	PathNode()                       : PathNode(nullptr)                                            {}
	PathNode(const MapNode* mapNode) : f_score(0), g_score(0), mParentNode(nullptr), mNode(mapNode) {}
};

class Pathfinder: public virtual MOAIEntity2D {
public:
	Pathfinder ();
	~Pathfinder();

	const USVec2D& GetStartPosition() const { return mStartPosition; }
	const USVec2D& GetEndPosition  () const { return mEndPosition;   }

	void SetCharacter(Character* character)  { mCharacter = character; mStartPosition = mCharacter->GetLoc(); mEndPosition = mCharacter->GetLoc(); UpdatePath(); }
	void SetStartPosition (float x, float y) { mStartPosition = USVec2D(x, y); UpdatePath(); }
	void SetEndPosition   (float x, float y) { mEndPosition   = USVec2D(x, y); UpdatePath(); }

	bool PathfindStep();
	virtual void DrawDebug();
private:
	void UpdatePath();
	PathNode popNodeWithMinCost();

	USVec2D                       mStartPosition;
	USVec2D                       mEndPosition;
	std::vector<MapNode*>         mMap;
	std::vector<const MapNode*>   mPath;
	map<const MapNode*, PathNode> mOpenList;
	map<const MapNode*, PathNode> mClosedList;
	Character*                    mCharacter;

	// Lua configuration
public:
	DECL_LUA_FACTORY(Pathfinder)
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setStartPosition(lua_State* L);
	static int _setEndPosition(lua_State* L);
	static int _pathfindStep(lua_State* L);
	static int _setCharacter(lua_State* L);
};

std::vector<USVec2D> generatePathFromGridNodes(std::vector<const MapNode*> nodes);
std::vector<USVec2D> generatePathFromPolygonNodes(std::vector<const MapNode*> nodes, const USVec2D& start, const USVec2D& end);
std::vector<USVec2D> generatePathFromPolygonNodesOptimizated(std::vector<const MapNode*> nodes, const USVec2D& start, const USVec2D& end);

#endif