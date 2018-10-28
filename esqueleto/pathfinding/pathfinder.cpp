#include <stdafx.h>

#include "pathfinder.h"
#include "character.h"
#include "map_node.h"
#include <fstream>

Pathfinder::Pathfinder() : MOAIEntity2D(), mStartPosition(0.0f, 0.0f), mEndPosition(0.0f, 0.0f), mCharacter(nullptr) {
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END

	//ReadGrid("grid_map.txt", mMap);
	ReadNavmesh("navmesh.xml", mMap);
}

Pathfinder::~Pathfinder() {

}

void Pathfinder::UpdatePath() {
	mPath.clear();
	mOpenList.clear();
	mClosedList.clear();

	const MapNode* nodeOfPoint = MapNode::getNodeOfPoint(mStartPosition, mMap);
	if (!nodeOfPoint) {
		return;
	}
	mOpenList[nodeOfPoint] = PathNode(nodeOfPoint);

	nodeOfPoint = MapNode::getNodeOfPoint(mEndPosition, mMap);
	if (!nodeOfPoint) {
		return;
	}

	while (!PathfindStep());

	if (mCharacter)
	{
		std::vector<USVec2D> reverse;
		reverse.push_back(mStartPosition);
		for (size_t i = 0; i + 1 < mPath.size(); i++)
		{
			const MapNode* n1 = mPath[mPath.size() - i - 1];
			const MapNode* n2 = mPath[mPath.size() - i - 2];

			reverse.push_back(n1->getPathPoint(n2));
		}
		reverse.push_back(mEndPosition);
		mCharacter->SetPath(reverse);
	}
}

void Pathfinder::DrawDebug()
{
	USVec2D offset(1024, 768);
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	USRect kk = {-512,512, -384,384};
	//MOAIDraw::DrawGrid(kk, 32, 24);

	//for (int row = 0; row < MAP_ROWS; row++)
	//{
	//	for (int column = 0; column < MAP_COLUMNS; column++)
	//	{
	//		if (m_map[row][column] == '#')
	//		{
	//			gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	//			USVec2D origin(column * 32 - 512, row * 32 - 384);
	//			MOAIDraw::DrawRectFill(origin.mX, origin.mY, origin.mX + 32, origin.mY + 32);
	//		}
	//		if (m_map[row][column] == 'o')
	//		{
	//			gfxDevice.SetPenColor(0.0f, 0.0f, 0.5f, 0.5f);
	//			USVec2D origin(column * 32 - 512, row * 32 - 384);
	//			MOAIDraw::DrawRectFill(origin.mX, origin.mY, origin.mX + 32, origin.mY + 32);
	//		}
	//	}
	//}
	//
	//gfxDevice.SetPenColor(0.0f, 1.0f, 1.0f, 0.5f);
	//for (auto it = openList.begin(); it != openList.end(); ++it)
	//{
	//	int cellX = it->second.mPos.mX * GRID_SIZE - 384;
	//	int cellY = it->second.mPos.mY * GRID_SIZE - 512;
	//	MOAIDraw::DrawRectFill(cellY, cellX, cellY + GRID_SIZE, cellX + GRID_SIZE);
	//}

	//gfxDevice.SetPenColor(0.5f, 0.0f, 0.0f, 0.5f);
	//for (auto it = closedList.begin(); it != closedList.end(); ++it)
	//{
	//	int cellX = it->second.mPos.mX * GRID_SIZE - 384;
	//	int cellY = it->second.mPos.mY * GRID_SIZE - 512;
	//	MOAIDraw::DrawRectFill(cellY, cellX, cellY + GRID_SIZE, cellX + GRID_SIZE);
	//}

	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
	for (int i = 0; i + 1 < mPath.size(); i++)
	{
		//MOAIDraw::DrawLine(m_path[i], m_path[i + 1]);
	}

	//USRect kk = { -512,512, -384,384 };
	//gfxDevice.SetPenColor(0.0f, 0.4f, 0.0f, 0.5f);
	//MOAIDraw::DrawGrid(kk, 32, 24);

	//gfxDevice.SetPointSize(5.0f);
	//MOAIDraw::DrawPoint(m_StartPosition);
	//MOAIDraw::DrawPoint(m_EndPosition);

	//Draw Navmesh
	for (int i = 0; i < mMap.size(); i++)
	{
		mMap[i]->DrawDebug(mPath);
	}

}

bool Pathfinder::PathfindStep()
{
    // returns true if pathfinding process finished
	if (!mOpenList.empty())
	{
		PathNode currentNode = popNodeWithMinCost();
		mPath.clear();
		const MapNode* parentId = currentNode.mParentNode;
		mPath.push_back(currentNode.mNode);
		while (parentId) {
			mPath.push_back(mClosedList[parentId].mNode);
			parentId = mClosedList[parentId].mParentNode;
		}

		if (currentNode.mNode == MapNode::getNodeOfPoint(mEndPosition, mMap)) {
			mOpenList.clear();
			mClosedList.clear();
			return true;
		}
		else {
			USVec2D initPos = mStartPosition;
			if (currentNode.mParentNode) {
				initPos = currentNode.mNode->getPathPoint(currentNode.mParentNode);
			}
			mOpenList.erase(currentNode.mNode);
			mClosedList[currentNode.mNode] = currentNode;
			const MapNode* neighbor = currentNode.mNode->getNextNeighbor(nullptr);
			while (neighbor) {
				if (!mClosedList.count(neighbor)) {
					PathNode neighborNode(neighbor);
					USVec2D endPos = currentNode.mNode->getPathPoint(neighbor);

					neighborNode.mParentNode = currentNode.mNode;
					neighborNode.g_score = currentNode.g_score + currentNode.mNode->getCostToNeighbor(initPos, neighbor);
					neighborNode.f_score = neighborNode.g_score + endPos.Dist(mEndPosition);

					if (mOpenList.count(neighbor)) {
						if (neighborNode.g_score < mOpenList[neighbor].g_score) {
							mOpenList[neighbor] = neighborNode;
						}
					}
					else {
						mOpenList[neighbor] = neighborNode;
					}
				}
				neighbor = currentNode.mNode->getNextNeighbor(neighbor);
			}



			//for (size_t i = 0; i < currentNode.mPolygon.mEdges.size() ; ++i)
			//{
			//	PathNode neighbor(*currentNode.mPolygon.mEdges[i].mNeighbour);
			//	//neighbor.mPos = neighbor.mPolygon.mVerts[currentNode.mPolygon.mEdges[i].mVerts[0]];
			//	//neighbor.mPos = compute2DPolygonCentroid(&neighbor.mPolygon.mVerts[0], neighbor.mPolygon.mVerts.size());
			//	USVec2D segment = currentNode.mPolygon.mEdges[i].mVerts[0] - currentNode.mPolygon.mEdges[i].mVerts[1];
			//	neighbor.mPos = currentNode.mPolygon.mEdges[i].mVerts[1] + segment * 0.5f;

			//	//USVec2D segment2(0, 0);
			//	//USVec2D mPos2;
			//	//for (size_t j = 0; j < neighbor.mPolygon.mEdges.size(); j++)
			//	//{
			//	//	NavPolygon::Edge edge = neighbor.mPolygon.mEdges[j];
			//	//	if (reinterpret_cast<int>(edge.mNeighbour) == currentNode.id) {
			//	//		segment2 = currentNode.mPolygon.mVerts[edge.mVerts[0]] - currentNode.mPolygon.mVerts[edge.mVerts[1]];
			//	//		mPos2 = currentNode.mPolygon.mVerts[edge.mVerts[1]] + segment2 * 0.5f;
			//	//		break;
			//	//	}
			//	//}
			//	//if (segment2.LengthSquared() < segment.LengthSquared()) {
			//	//	neighbor.mPos = mPos2;
			//	//}
			//
			//	

			//	neighbor.parentId = currentNode.id;
			//	neighbor.g_score = currentNode.g_score + 1;
			//	neighbor.f_score = neighbor.g_score + (endNode.mPos - neighbor.mPos).Length();
			//	if (closedList.count(neighbor.id)) {
			//		continue;
			//	}
			//	if (openList.count(neighbor.id)) {
			//		if (neighbor.g_score < openList[neighbor.id].g_score) {
			//			openList[neighbor.id] = neighbor;
			//		}
			//	}
			//	else {
			//		openList[neighbor.id] = neighbor;
			//	}
			//}


			//for (int row = -1; row <= 1; ++row) {
			//	for (int column = -1; column <= 1; ++column) {
			//		if ((row != 0 || column != 0)
			//			&& (currentNode.mPos.mX + row >= 0)
			//			&& (currentNode.mPos.mX + row < MAP_ROWS)
			//			&& (currentNode.mPos.mY + column >= 0)
			//			&& (currentNode.mPos.mY + column < MAP_COLUMNS)) {
			//			float factor = 1.0f;
			//			if (row != 0 && column != 0 ) {
			//				factor = 1.4f;
			//			}
			//			PathNode neighbor(USVec2D(currentNode.mPos.mX + row, currentNode.mPos.mY + column));
			//			neighbor.parentId = currentNode.id;
			//			if (m_map[(int)neighbor.mPos.mX][(int)neighbor.mPos.mY] == '#') {
			//				neighbor.g_score = 9999999;
			//			}
			//			else if (m_map[(int)neighbor.mPos.mX][(int)neighbor.mPos.mY] == 'o') {
			//				neighbor.g_score = currentNode.g_score + 3 * factor; // agua
			//			}
			//			else {
			//				neighbor.g_score = currentNode.g_score + 1 * factor; //casillas colindantes tiene coste 1
			//			}
			//			neighbor.f_score = neighbor.g_score + (endNode.mPos - neighbor.mPos).Length();
			//			if (closedList.count(neighbor.id)) {
			//				continue;
			//			}
			//			if (openList.count(neighbor.id)) {
			//				if (neighbor.g_score < openList[neighbor.id].g_score) {
			//					openList[neighbor.id] = neighbor;
			//				}
			//			}
			//			else {
			//				openList[neighbor.id] = neighbor;
			//			}
			//		}
			//	}
			//}
		}
	}
	else
	{
		const MapNode* nodeOfPoint = MapNode::getNodeOfPoint(mStartPosition, mMap);
		mOpenList[nodeOfPoint] = PathNode(nodeOfPoint);
		PathfindStep();
	}

    return false;
}

PathNode Pathfinder::popNodeWithMinCost()
{
	float minCost = 9999999999.0f;
	PathNode popIndex;
	for (auto it = mOpenList.begin(); it != mOpenList.end(); ++it)
	{
		if (it->second.f_score < minCost)
		{
			minCost = it->second.f_score;
			popIndex = it->second;
		}
	}
	return popIndex;
}

//lua configuration ----------------------------------------------------------------//
void Pathfinder::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity::RegisterLuaFuncs(state);

	luaL_Reg regTable [] = {
		{ "setStartPosition",		_setStartPosition},
		{ "setEndPosition",			_setEndPosition},
        { "pathfindStep",           _pathfindStep},
		{ "setCharacter",           _setCharacter},
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

int Pathfinder::_setCharacter(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

	self->SetCharacter(state.GetLuaObject<Character>(2, 0.0f));
	return 0;
}
