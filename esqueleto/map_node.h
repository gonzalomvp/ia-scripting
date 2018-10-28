#pragma once

#include <array>
#include <vector>
#include <map>

struct MapNode {
	virtual bool isPointInNode(const USVec2D& point) const = 0;
	virtual USVec2D getPathPoint(const MapNode* neighbor) const = 0;
	virtual const MapNode* getNextNeighbor(const MapNode* previous) const = 0;

	static const MapNode* getNodeOfPoint(const USVec2D& point, const std::vector<MapNode*>& map);
};

struct GridNode : public MapNode {
	virtual bool isPointInNode(const USVec2D& point) const override { return false; }
	virtual USVec2D getPathPoint(const MapNode* neighbor) const override { return USVec2D(); }
};

struct NavPolygon : public MapNode {
	virtual bool isPointInNode(const USVec2D& point) const override;
	virtual USVec2D getPathPoint(const MapNode* neighbor) const override;
	virtual const MapNode* getNextNeighbor(const MapNode* previous) const;


	std::vector<USVec2D> mVerts;
	std::map<const MapNode*, std::array<USVec2D, 2>> mEdges;
};