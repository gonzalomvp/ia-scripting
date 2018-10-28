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

void NavPolygon::DrawDebug() {
	//MOAIDraw::DrawPolygon(mVerts);
}