#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>

#define MAP_ROWS    24
#define MAP_COLUMNS 32
#define GRID_SIZE   32

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
};


#endif