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
	for (size_t i = 0; i < mMap.size(); ++i) {
		delete mMap[i];
	}
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

	if (mCharacter) {
		std::vector<USVec2D> reverse;
		std::vector<std::array<USVec2D, 2>> edges;
		for (size_t i = 0; i + 1 < mPath.size(); i++) {
			const NavPolygon* node1 = reinterpret_cast<const NavPolygon*>(mPath[mPath.size() - i - 1]);
			const NavPolygon* node2 = reinterpret_cast<const NavPolygon*>(mPath[mPath.size() - i - 2]);
			std::map<const MapNode*, std::array<USVec2D, 2>> tempEdges = node1->mEdges;
			edges.push_back(tempEdges[node2]);
		}

		USVec2D currentPosition = mStartPosition;
		reverse.push_back(currentPosition);
		for (size_t i = 0; i < edges.size(); ++i) {
			if (get_line_intersection(currentPosition.mX, currentPosition.mY, mEndPosition.mX, mEndPosition.mY, edges[i][0].mX, edges[i][0].mY, edges[i][1].mX, edges[i][1].mY, &currentPosition.mX, &currentPosition.mY)) {
				//reverse.push_back(currentPosition);
			}
			else {
				USVec2D right = edges[i][0];
				USVec2D left = edges[i][1];
				USVec2D v1 = right - currentPosition;
				USVec2D v2 = left - currentPosition;
				
				float dir = v1.Cross(v2);
				if (dir < 0.0f) {
					v1 = v2;
					right = edges[i][1];
					left = edges[i][0];
				}
				v2 = mEndPosition - currentPosition;
				dir = v1.Cross(v2);
				if (dir < 0.0f) {
					currentPosition = right;
				}
				else {
					currentPosition = left;
				}
				reverse.push_back(currentPosition);
			}
		}
		reverse.push_back(mEndPosition);
		mCharacter->SetPath(reverse);
	}
}

void Pathfinder::DrawDebug() {
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

	//Draw Map Nodes
	for (int i = 0; i < mMap.size(); i++) {
		mMap[i]->DrawDebug();
	}

	//Draw Open list in light blue
	for (auto it = mOpenList.begin(); it != mOpenList.end(); ++it) {
		gfxDevice.SetPenColor(0.0f, 1.0f, 1.0f, 0.5f);
		it->first->DrawFill();
	}

	//Draw Closed list in orange
	for (auto it = mClosedList.begin(); it != mClosedList.end(); ++it) {
		gfxDevice.SetPenColor(1.0f, 0.5f, 0.0f, 0.5f);
		it->first->DrawFill();
	}

	//Draw Path in green only if open list is not empty
	if (mOpenList.size() > 0) {
		for (int i = 0; i < mPath.size(); i++) {
			gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
			mPath[i]->DrawFill();
		}
	}
	
}

bool Pathfinder::PathfindStep() {
    // returns true if pathfinding process finishes
	if (!mOpenList.empty()) {
		PathNode currentNode = popNodeWithMinCost();

		//Abort if not possible to get node from list
		if (!currentNode.mNode) {
			return true;
		}

		const MapNode* parentId = currentNode.mParentNode;

		//Build path
		mPath.clear();
		mPath.push_back(currentNode.mNode);
		while (parentId) {
			mPath.push_back(mClosedList[parentId].mNode);
			parentId = mClosedList[parentId].mParentNode;
		}

		//Check if arrived to end node
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

			//Calculate cost for each neighbor
			const MapNode* neighbor = currentNode.mNode->getNextNeighbor(nullptr);
			while (neighbor) {
				if (!mClosedList.count(neighbor)) {
					PathNode neighborNode(neighbor);
					USVec2D endPos = currentNode.mNode->getPathPoint(neighbor);

					neighborNode.mParentNode = currentNode.mNode;
					neighborNode.g_score = currentNode.g_score + currentNode.mNode->getCostToNeighbor(initPos, neighbor);
					neighborNode.f_score = neighborNode.g_score + endPos.Dist(mEndPosition);

					if (mOpenList.count(neighbor)) {
						//Update cost if lower
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
		}
	}
	else {
		const MapNode* nodeOfPoint = MapNode::getNodeOfPoint(mStartPosition, mMap);
		mOpenList[nodeOfPoint] = PathNode(nodeOfPoint);
		PathfindStep();
	}
    return false;
}

PathNode Pathfinder::popNodeWithMinCost() {
	PathNode minCostNode;
	float minCost = 0.0f;

	if (mOpenList.begin() != mOpenList.end()) {
		minCostNode = mOpenList.begin()->second;
		minCost = mOpenList.begin()->second.f_score;
		for (auto it = mOpenList.begin(); it != mOpenList.end(); ++it) {
			if (it->second.f_score < minCost) {
				minCostNode = it->second;
				minCost = it->second.f_score;
			}
		}
	}
	
	return minCostNode;
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

bool get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
	float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	float s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		if (i_x != NULL)
			*i_x = p0_x + (t * s1_x);
		if (i_y != NULL)
			*i_y = p0_y + (t * s1_y);
		return true;
	}

	return false; // No collision
}