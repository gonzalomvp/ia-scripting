#pragma once

#include <array>
#include <vector>
#include <map>

#define MAP_ROWS    24
#define MAP_COLUMNS 32
#define GRID_SIZE   32

struct MapNode {
	virtual bool isPointInNode(const USVec2D& point) const = 0;
	virtual USVec2D getPathPoint(const MapNode* neighbor) const = 0;
	virtual const MapNode* getNextNeighbor(const MapNode* previous) const = 0;
	virtual float getCostToNeighbor(USVec2D& init, const MapNode* neighbor) const = 0;
	virtual void DrawDebug(const std::vector<const MapNode*>& map) = 0;

	static const MapNode* getNodeOfPoint(const USVec2D& point, const std::vector<MapNode*>& map);
};

struct GridNode : public MapNode {
	virtual bool isPointInNode(const USVec2D& point) const override;
	virtual USVec2D getPathPoint(const MapNode* neighbor) const override;
	virtual const MapNode* getNextNeighbor(const MapNode* previous) const override;
	virtual float getCostToNeighbor(USVec2D& init, const MapNode* neighbor) const override;
	virtual void DrawDebug(const std::vector<const MapNode*>& map) override;

	int mIndex;
	int mCost;
	std::vector<GridNode*> mNeighbors;
	//costToNeig??
};

struct NavPolygon : public MapNode {
	virtual bool isPointInNode(const USVec2D& point) const override;
	virtual USVec2D getPathPoint(const MapNode* neighbor) const override;
	virtual const MapNode* getNextNeighbor(const MapNode* previous) const;
	virtual float getCostToNeighbor(USVec2D& init, const MapNode* neighbor) const override;
	virtual void DrawDebug(const std::vector<const MapNode*>& map) override;

	std::vector<USVec2D> mVerts;
	std::map<const MapNode*, std::array<USVec2D, 2>> mEdges;
};