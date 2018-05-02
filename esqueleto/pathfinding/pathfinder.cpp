#include <stdafx.h>

#include "pathfinder.h"
#include <fstream>

struct PathNode
{
	int id;
	USVec2D mPos;
	float g_score;
	float f_score;
	int parentId;
	
	PathNode() {}
	PathNode(const USVec2D& pos) : f_score(0), g_score(0), parentId(-1) { mPos = pos; id = mPos.mX * MAP_COLUMNS + mPos.mY; }
	

	bool operator==(const PathNode& other) const{ return id == other.id; }
};

PathNode popNodeWithMinCost(map<int, PathNode> openlist);

Pathfinder::Pathfinder() : MOAIEntity2D()
{
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END

	//Read map file
	std::ifstream file("map.txt", std::ios::binary);
	std::string line;

	for (size_t row = 0; row < MAP_ROWS; row++)
	{
		getline(file, line);
		for (size_t column = 0; column < MAP_COLUMNS; column++)
		{
			m_map[row][column] = line[column];
		}
	}
	file.close();
	m_StartPosition = USVec2D(0.0f, 0.0f);
	m_EndPosition   = USVec2D(0.0f, 0.0f);
}

Pathfinder::~Pathfinder()
{

}

void Pathfinder::UpdatePath()
{
	m_path.clear();
	USVec2D origin(-512, -384);
	PathNode startNode(USVec2D(floorf((m_StartPosition.mY - origin.mY) / GRID_SIZE), floorf((m_StartPosition.mX - origin.mX) / GRID_SIZE)));
	PathNode endNode(USVec2D(floorf((m_EndPosition.mY - origin.mY) / GRID_SIZE), floorf((m_EndPosition.mX - origin.mX) / GRID_SIZE)));

	map<int, PathNode> closedList;
	map<int, PathNode> openList;

	openList[startNode.id] = startNode;

	while (!openList.empty())
	{
		PathNode currentNode = popNodeWithMinCost(openList);
		if (currentNode == endNode) {
			m_path.push_back(currentNode.mPos);
			int parentId = currentNode.parentId;
			while (parentId != -1) {
				m_path.push_back(closedList[parentId].mPos);
				parentId = closedList[parentId].parentId;
			}
			break;
		}
		else {
			openList.erase(currentNode.id);
			closedList[currentNode.id] = currentNode;
			for (int row = -1; row <= 1; ++row) {
				for (int column = -1; column <= 1; ++column) {
					if ((row != 0 || column != 0) 
						&& (currentNode.mPos.mX + row >= 0)
						&& (currentNode.mPos.mX + row < MAP_ROWS)
						&& (currentNode.mPos.mY + column >= 0)
						&& (currentNode.mPos.mY + column < MAP_COLUMNS)) {
						PathNode neighbor(USVec2D(currentNode.mPos.mX + row, currentNode.mPos.mY + column));
						neighbor.parentId = currentNode.id;
						neighbor.g_score = currentNode.g_score + 1; //casillas colindantes tiene coste 1
						neighbor.f_score = neighbor.g_score + (endNode.mPos - neighbor.mPos).Length();
						if (closedList.count(neighbor.id)) {
							continue;
						}
						if (openList.count(neighbor.id)) {
							if (neighbor.g_score < openList[neighbor.id].g_score) {
								openList[neighbor.id] = neighbor;
							}
						}
						else {
							openList[neighbor.id] = neighbor;
						}
					}
				}
			}
		}
	}
}

void Pathfinder::DrawDebug()
{
	USVec2D offset(1024, 768);
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	USRect kk = {-512,512, -384,384};
	MOAIDraw::DrawGrid(kk, 32, 24);

	for (int row = 0; row < MAP_ROWS; row++)
	{
		for (int column = 0; column < MAP_COLUMNS; column++)
		{
			if (m_map[row][column] == '#')
			{
				USVec2D origin(column * 32 - 512, row * 32 - 384);
				MOAIDraw::DrawRectFill(origin.mX, origin.mY, origin.mX + 32, origin.mY + 32);
			}
		}
	}
	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
	for (int i = 0; i < m_path.size(); i ++) {
		int cellX = m_path[i].mX * GRID_SIZE - 384;
		int cellY = m_path[i].mY * GRID_SIZE - 512;
		MOAIDraw::DrawRectFill(cellY, cellX, cellY + GRID_SIZE, cellX + GRID_SIZE);
	}

	gfxDevice.SetPointSize(5.0f);
	MOAIDraw::DrawPoint(m_StartPosition);
	MOAIDraw::DrawPoint(m_EndPosition);
}

bool Pathfinder::PathfindStep()
{
    // returns true if pathfinding process finished
    return true;
}















//lua configuration ----------------------------------------------------------------//
void Pathfinder::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity::RegisterLuaFuncs(state);

	luaL_Reg regTable [] = {
		{ "setStartPosition",		_setStartPosition},
		{ "setEndPosition",			_setEndPosition},
        { "pathfindStep",           _pathfindStep},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Pathfinder::_setStartPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetStartPosition(pX, pY);
	return 0;
}

int Pathfinder::_setEndPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetEndPosition(pX, pY);
	return 0;
}

int Pathfinder::_pathfindStep(lua_State* L)
{
    MOAI_LUA_SETUP(Pathfinder, "U")

    self->PathfindStep();
    return 0;
}


PathNode popNodeWithMinCost(map<int, PathNode> openlist)
{
	int minCost = 99999999;
	int popIndex = -1;
	for (auto it = openlist.begin(); it != openlist.end(); ++it)
	{
		if (it->second.f_score < minCost)
		{
			minCost = it->second.f_score;
			popIndex = it->first;
		}
	}
	return openlist[popIndex];
}