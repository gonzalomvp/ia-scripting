#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>

#define MAP_ROWS    24
#define MAP_COLUMNS 32
#define GRID_SIZE   32

struct PathNode
{
	int id;
	USVec2D mPos;
	float g_score;
	float f_score;
	int parentId;

	PathNode() {}
	PathNode(const USVec2D& pos) : f_score(0), g_score(0), parentId(-1) { mPos = pos; id = mPos.mX * MAP_COLUMNS + mPos.mY; }


	bool operator==(const PathNode& other) const { return id == other.id; }
};

PathNode popNodeWithMinCost(map<int, PathNode> openlist);

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
	USVec2D m_StartPosition;
	USVec2D m_EndPosition;

	// Lua configuration
public:
	DECL_LUA_FACTORY(Pathfinder)
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setStartPosition(lua_State* L);
	static int _setEndPosition(lua_State* L);
    static int _pathfindStep(lua_State* L);

	char m_map[MAP_ROWS][MAP_COLUMNS];
	std::vector<USVec2D> m_path;
	map<int, PathNode> closedList;
	map<int, PathNode> openList;
	PathNode startNode;
	PathNode endNode;
};


#endif