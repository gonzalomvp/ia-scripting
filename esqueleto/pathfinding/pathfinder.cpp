#include <stdafx.h>

#include "pathfinder.h"
#include "character.h"
#include "map_node.h"
#include <fstream>

Pathfinder::Pathfinder() : MOAIEntity2D(), mStartPosition(0.0f, 0.0f), mEndPosition(0.0f, 0.0f), mCharacter(nullptr) {
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END

	ReadGrid("grid_map.txt", mMap);
	//ReadNavmesh("navmesh.xml", mMap);
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
		reverse.push_back(mStartPosition);
		for (size_t i = 0; i + 1 < mPath.size(); i++) {
			const MapNode* node1 = mPath[mPath.size() - i - 1];
			const MapNode* node2 = mPath[mPath.size() - i - 2];

			reverse.push_back(node1->getPathPoint(node2));
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
