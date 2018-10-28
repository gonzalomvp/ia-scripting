#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>
#include "params.h"

class Character;
class MapNode;



struct PathNode
{
	int id;
	USVec2D mPos;
	NavPolygon mPolygon;
	const MapNode* mMapNode;
	float g_score;
	float f_score;
	const MapNode* parentId;

	PathNode() {}
	PathNode(const NavPolygon& polygon) : f_score(0), g_score(0), parentId(nullptr), id(reinterpret_cast<int>(&polygon)), mPolygon(polygon) {}
	PathNode(const MapNode* mapNode) : f_score(0), g_score(0), parentId(nullptr), mMapNode(mapNode) {}


	bool operator==(const PathNode& other) const { return mMapNode == other.mMapNode; }
};

PathNode popNodeWithMinCost(map<const MapNode*, PathNode> openlist);

class Pathfinder: public virtual MOAIEntity2D
{
public:
	Pathfinder();
	~Pathfinder();

	virtual void DrawDebug();

	void SetStartPosition(float x, float y) { m_StartPosition = USVec2D(x, y); UpdatePath();}
	void SetEndPosition(float x, float y) { m_EndPosition = USVec2D(x, y); UpdatePath();}
	const USVec2D& GetStartPosition() const { return m_StartPosition;}
	const USVec2D& GetEndPosition() const { return m_EndPosition;}

    bool PathfindStep();
private:
	void UpdatePath();
private:
	USVec2D              m_StartPosition;
	USVec2D              m_EndPosition;
	std::vector<MapNode*> mNavmesh;


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

	std::vector<const MapNode*> m_path;
	map<const MapNode*, PathNode> closedList;
	map<const MapNode*, PathNode> openList;
	PathNode startNode;
	PathNode endNode;
	Character* mCharacter;
};


#endif