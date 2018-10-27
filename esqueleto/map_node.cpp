#include <stdafx.h>

#include "map_node.h"


const MapNode* MapNode::getNodeOfPoint(const USVec2D& point, const std::vector<const MapNode*>& map) {
	const MapNode* node = nullptr;
	for (size_t i = 0; i < map.size(); ++i) {
		if (map[i]->isPointInNode(point)) {
			node = map[i];
		}
	}
	return node;
}


//const std::vector<const MapNode*>& NavPolygon::getNeighbors() const {
//	for (auto it = mEdges.begin(); it != mEdges.end(); ++it) {
//
//	}
//
//}

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