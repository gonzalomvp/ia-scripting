#include <stdafx.h>

#include "pathfinder.h"
#include <fstream>

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
}

Pathfinder::~Pathfinder()
{

}

void Pathfinder::UpdatePath()
{
	USVec2D offset(512, 384);
	vector<USVec2D> open;
	vector<USVec2D> closed;
	int costs[MAP_ROWS][MAP_COLUMNS];
	for (size_t row = 0; row < MAP_ROWS; row++)
	{
		for (size_t column = 0; column < MAP_COLUMNS; column++)
		{
			costs[row][column] = 1;
		}
	}
	USVec2D startPoint = m_StartPosition + offset;
	USVec2D endPoint = m_EndPosition+ offset;
	int startRow = startPoint.mY / GRID_SIZE;
	int startColumm = startPoint.mX / GRID_SIZE;
	int endRow = endPoint.mY / GRID_SIZE;
	int endColumm = endPoint.mX / GRID_SIZE;
	USVec2D goal(endRow, endColumm);

	if (endRow >= 0 && endRow < MAP_ROWS && endColumm >= 0 && endColumm < MAP_COLUMNS)
	{
		open.push_back(USVec2D(startRow, startColumm));
		costs[startRow][startColumm] = 1;
		while (!open.empty())
		{
			int minCost = 99999999;
			int popIndex = -1;
			for (size_t i = 0; i < open.size(); i++)
			{
				if (costs[(int)open[i].mX][(int)open[i].mY] < minCost)
				{
					minCost = costs[(int)open[i].mX][(int)open[i].mY];
					popIndex = i;
				}
			}
			USVec2D closest = open[popIndex];
			if (closest.mX == endRow && closest.mY == endColumm)
			{

			}
			else
			{
				closed.push_back(closest);
				for (auto node = open.begin(); node != open.end(); ++node)
				{
					if ((*node).mX == closest.mX && (*node).mY == closest.mY)
					{
						open.erase(node);
						break;
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