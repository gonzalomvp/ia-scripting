#include <stdafx.h>

#include "pathfinder.h"
#include "character.h"
#include "map_node.h"
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
	m_StartPosition = USVec2D(0.0f, 0.0f);
	m_EndPosition   = USVec2D(0.0f, 0.0f);
	mCharacter = nullptr;
	ReadNavmesh("navmesh.xml", mNavmesh);
}

Pathfinder::~Pathfinder()
{

}

void Pathfinder::UpdatePath()
{
	m_path.clear();
	openList.clear();
	closedList.clear();
	USVec2D origin(-512, -384);
	startNode = PathNode(mNavmesh[14]);
	endNode = PathNode(mNavmesh[57]);

	const MapNode* nodeOfPoint = MapNode::getNodeOfPoint(m_StartPosition, mNavmesh);
	if (nodeOfPoint) {
		startNode = PathNode(nodeOfPoint);
	}
	nodeOfPoint = MapNode::getNodeOfPoint(m_EndPosition, mNavmesh);
	if (nodeOfPoint) {
		endNode = PathNode(nodeOfPoint);
	}


	//for (size_t i = 0; i < mNavmesh.size(); i++)
	//{
	//	//if (cn_PnPoly(m_StartPosition, &mNavmesh[i].mVerts[0], mNavmesh[i].mVerts.size())) {
	//	if(PointInPolygon(m_StartPosition, *mNavmesh[i])){
	//		startNode = PathNode(mNavmesh[i]);
	//		startNode.mPos = m_StartPosition;
	//		printf("START: %d\n", i);
	//		break;
	//	}
	//}
	//for (size_t i = 0; i < mNavmesh.size(); i++)
	//{
	//	//if (cn_PnPoly(m_EndPosition, &mNavmesh[i].mVerts[0], mNavmesh[i].mVerts.size())) {
	//	if (PointInPolygon(m_EndPosition, *mNavmesh[i])) {
	//		endNode = PathNode(mNavmesh[i]);
	//		endNode.mPos = m_EndPosition;
	//		printf("END: %d\n", i);
	//		break;
	//	}
	//}

	openList[startNode.mMapNode] = startNode;

	while (!PathfindStep());

	if (mCharacter)
	{
		std::vector<USVec2D> reverse;
		reverse.push_back(m_StartPosition);
		for (size_t i = 0; i + 1 < m_path.size(); i++)
		{
			const MapNode* n1 = m_path[m_path.size() - i - 1];
			const MapNode* n2 = m_path[m_path.size() - i - 2];

			reverse.push_back(n1->getPathPoint(n2));
		}
		reverse.push_back(m_EndPosition);
		mCharacter->SetPath(reverse);
	}
	

	//while (!openList.empty())
	//{
	//	PathNode currentNode = popNodeWithMinCost(openList);
	//	if (currentNode == endNode) {
	//		m_path.push_back(currentNode.mPos);
	//		int parentId = currentNode.parentId;
	//		while (parentId != -1) {
	//			m_path.push_back(closedList[parentId].mPos);
	//			parentId = closedList[parentId].parentId;
	//		}
	//		break;
	//	}
	//	else {
	//		openList.erase(currentNode.id);
	//		closedList[currentNode.id] = currentNode;
	//		for (int row = -1; row <= 1; ++row) {
	//			for (int column = -1; column <= 1; ++column) {
	//				if ((row != 0 || column != 0) 
	//					&& (currentNode.mPos.mX + row >= 0)
	//					&& (currentNode.mPos.mX + row < MAP_ROWS)
	//					&& (currentNode.mPos.mY + column >= 0)
	//					&& (currentNode.mPos.mY + column < MAP_COLUMNS)) {
	//					PathNode neighbor(USVec2D(currentNode.mPos.mX + row, currentNode.mPos.mY + column));
	//					neighbor.parentId = currentNode.id;
	//					if (m_map[(int)neighbor.mPos.mX][(int)neighbor.mPos.mY] == '#') {
	//						neighbor.g_score = 9999999;
	//					}
	//					else if (m_map[(int)neighbor.mPos.mX][(int)neighbor.mPos.mY] == 'o') {
	//						neighbor.g_score = currentNode.g_score + 3; // agua
	//					}
	//					else {
	//						neighbor.g_score = currentNode.g_score + 1; //casillas colindantes tiene coste 1
	//					}
	//					neighbor.f_score = neighbor.g_score + (endNode.mPos - neighbor.mPos).Length();
	//					if (closedList.count(neighbor.id)) {
	//						continue;
	//					}
	//					if (openList.count(neighbor.id)) {
	//						if (neighbor.g_score < openList[neighbor.id].g_score) {
	//							openList[neighbor.id] = neighbor;
	//						}
	//					}
	//					else {
	//						openList[neighbor.id] = neighbor;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

void Pathfinder::DrawDebug()
{
	USVec2D offset(1024, 768);
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	//USRect kk = {-512,512, -384,384};
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
	for (int i = 0; i + 1 < m_path.size(); i++)
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
	for (int i = 0; i < mNavmesh.size(); i++)
	{
		NavPolygon polygon = *reinterpret_cast<NavPolygon*>(mNavmesh[i]);
		for (int j = 0; j + 1 < polygon.mVerts.size(); ++j) {
			
			//MOAIDraw::DrawLine(polygon.mVerts[j], polygon.mVerts[j + 1]);
		}
		//MOAIDraw::DrawLine(polygon.points[0], polygon.points[polygon.points.size() - 1]);
		gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
		//MOAIDraw::DrawPolygon(polygon.mVerts);

		gfxDevice.SetPenColor(0.0f, 1.0f, 1.0f, 0.1f);
		if (std::find(m_path.begin(), m_path.end(), mNavmesh[i]) != m_path.end())
		{
			gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.05f);
		}

		
		//MOAIDraw::DrawPolygonFilled(polygon.mVerts);
	}

}

bool Pathfinder::PathfindStep()
{
    // returns true if pathfinding process finished
	if (!openList.empty())
	{
		PathNode currentNode = popNodeWithMinCost(openList);
		m_path.clear();
		const MapNode* parentId = currentNode.parentId;
		m_path.push_back(currentNode.mMapNode);
		while (parentId) {
			m_path.push_back(closedList[parentId].mMapNode);
			parentId = closedList[parentId].parentId;
		}

		if (currentNode == endNode) {
			openList.clear();
			closedList.clear();
			return true;
		}
		else {
			openList.erase(currentNode.mMapNode);
			closedList[currentNode.mMapNode] = currentNode;
			const MapNode* neighbor = currentNode.mMapNode->getNextNeighbor(nullptr);
			while (neighbor) {
				if (!closedList.count(neighbor)) {
					PathNode neighborNode(neighbor);
					neighborNode.parentId = currentNode.mMapNode;
					neighborNode.g_score = currentNode.g_score + 1;
					neighborNode.f_score = neighborNode.g_score /*+ (endNode.mPos - neighborNode.mPos).Length()*/;

					if (openList.count(neighbor)) {
						if (neighborNode.g_score < openList[neighbor].g_score) {
							openList[neighbor] = neighborNode;
						}
					}
					else {
						openList[neighbor] = neighborNode;
					}
				}
				neighbor = currentNode.mMapNode->getNextNeighbor(neighbor);
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
		openList[startNode.mMapNode] = startNode;
		PathfindStep();
	}

    return false;
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

	self->mCharacter = state.GetLuaObject<Character>(2, 0.0f);
	return 0;
}


PathNode popNodeWithMinCost(map<const MapNode*, PathNode> openlist)
{
	float minCost = 99999999.0f;
	PathNode popIndex;
	for (auto it = openlist.begin(); it != openlist.end(); ++it)
	{
		if (it->second.f_score < minCost)
		{
			minCost = it->second.f_score;
			popIndex = it->second;
		}
	}
	return popIndex;
}