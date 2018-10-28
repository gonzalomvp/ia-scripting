#include <stdafx.h>

#include "map_node.h"
#include "params.h"

const MapNode* MapNode::getNodeOfPoint(const USVec2D& point, const std::vector<MapNode*>& map) {
	const MapNode* node = nullptr;
	for (size_t i = 0; i < map.size(); ++i) {
		if (map[i]->isPointInNode(point)) {
			node = map[i];
		}
	}
	return node;
}

const MapNode* GridNode::getNextNeighbor(const MapNode* previous) const {
	const MapNode* node = nullptr;
	if (previous == nullptr) {
		if (mNeighbors.begin() != mNeighbors.end()) {
			node = *mNeighbors.begin();
		}
		return node;
	}

	for (auto it = mNeighbors.begin(); it != mNeighbors.end(); ++it) {
		if (*it == previous) {
			++it;
			if (it != mNeighbors.end()) {
				node = *it;
			}
			break;
		}
	}

	return node;
}

bool GridNode::isPointInNode(const USVec2D& point) const {
	USVec2D center = getPathPoint(this);

	return point.mX >= center.mX - GRID_SIZE * 0.5f && point.mX <= center.mX + GRID_SIZE * 0.5f && point.mY >= center.mY - GRID_SIZE * 0.5f && point.mY <= center.mY + GRID_SIZE * 0.5f;
}

USVec2D GridNode::getPathPoint(const MapNode* neighbor) const {
	const GridNode* node = reinterpret_cast<const GridNode*>(neighbor);
	int row = node->mIndex / MAP_COLUMNS;
	int column = node->mIndex % MAP_COLUMNS;

	USVec2D topLeft(-512, -384);

	USVec2D point;
	point.mX = topLeft.mX + GRID_SIZE * column + GRID_SIZE * 0.5f;
	point.mY = topLeft.mY + GRID_SIZE * row + GRID_SIZE * 0.5f;
	return point;
}

float GridNode::getCostToNeighbor(USVec2D& init, const MapNode* neighbor) const {
	const GridNode* node = reinterpret_cast<const GridNode*>(neighbor);
	return node->mCost * getPathPoint(this).Dist(getPathPoint(neighbor));
}

void GridNode::DrawDebug(const std::vector<const MapNode*>& map) {
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPointSize(1.0f);
	USVec2D center = getPathPoint(this);
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	MOAIDraw::DrawPoint(center);

	if (std::find(map.begin(), map.end(), this) != map.end()) {
		gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
		MOAIDraw::DrawRectFill(center.mX - GRID_SIZE * 0.5f, center.mY - GRID_SIZE * 0.5f, center.mX + GRID_SIZE * 0.5f, center.mY + GRID_SIZE * 0.5f);
	}
	else if (mCost == 3) {
		gfxDevice.SetPenColor(0.0f, 0.0f, 0.5f, 0.5f);
		MOAIDraw::DrawRectFill(center.mX - GRID_SIZE * 0.5f, center.mY - GRID_SIZE * 0.5f, center.mX + GRID_SIZE * 0.5f, center.mY + GRID_SIZE * 0.5f);
	}
	else if (mCost == 9999999) {
		gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
		MOAIDraw::DrawRectFill(center.mX - GRID_SIZE * 0.5f, center.mY - GRID_SIZE * 0.5f, center.mX + GRID_SIZE * 0.5f, center.mY + GRID_SIZE * 0.5f);
	}
}

bool NavPolygon::isPointInNode(const USVec2D& point) const  {
	return PointInPolygon(point, *this);
}

USVec2D NavPolygon::getPathPoint(const MapNode* neighbor) const {
	std::map<const MapNode*, std::array<USVec2D, 2>> edges = mEdges;

	USVec2D p1 = edges[neighbor][0];
	USVec2D p2 = edges[neighbor][1];

	return p1 + (p2 - p1) * 0.5;
}


const MapNode* NavPolygon::getNextNeighbor(const MapNode* previous) const {
	const MapNode* node = nullptr;
	if (previous == nullptr) {
		if (mEdges.begin() != mEdges.end()) {
			node = mEdges.begin()->first;
		}
		return node;
	}
	
	for (auto it = mEdges.begin(); it != mEdges.end(); ++it) {
		if (it->first == previous) {
			++it;
			if (it != mEdges.end()) {
				node = it->first;
			}
			break;
		}
	}

	return node;
}

float NavPolygon::getCostToNeighbor(USVec2D& init, const MapNode* neighbor) const {
	USVec2D end = getPathPoint(neighbor);
	return init.Dist(end);
}

void NavPolygon::DrawDebug(const std::vector<const MapNode*>& map) {
	MOAIDraw::DrawPolygon(mVerts);
}
