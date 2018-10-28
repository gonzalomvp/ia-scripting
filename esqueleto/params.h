#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "map_node.h"
#include <vector>

struct Params
{
    USVec2D targetPosition;
    float max_velocity;
    float max_acceleration;
    float dest_radius;
	float arrive_radius;

	float targetRotation;
	float max_angular_velocity;
	float max_angular_acceleration;
	float angular_dest_radius;
	float angular_arrive_radius;

	float look_ahead;
	float time_ahead;

	float enemy_speed;
	USVec2D enemy_minPosition;
	USVec2D enemy_maxPosition;

	float char_radius;
};

//struct NavPolygon {
//	struct Edge {
//		USVec2D mVerts[2];
//		NavPolygon* mNeighbour;
//	};
//	std::vector<USVec2D> mVerts;
//	std::vector<Edge> mEdges;
//};

bool ReadParams(const char* filename, Params& params);
bool ReadPath(const char* filename, std::vector<USVec2D>& path);
bool ReadObstacles(const char* filename, std::vector<USVec3D>& obstacles);
bool ReadNavmesh(const char* filename, std::vector<MapNode*>& polygons);

inline int isLeft(USVec2D P0, USVec2D P1, USVec2D P2)
{
	return ((P1.mX - P0.mX) * (P2.mY - P0.mY) - (P2.mX - P0.mX) * (P1.mY - P0.mY));
}

inline int cn_PnPoly(USVec2D P, USVec2D* V, int n)
{
	int    cn = 0;    // the  crossing number counter

					  // loop through all edges of the polygon
	for (int i = 0; i<n; i++) {    // edge from V[i]  to V[i+1]
		if (((V[i].mY <= P.mY) && (V[i + 1].mY > P.mY))     // an upward crossing
			|| ((V[i].mY > P.mY) && (V[i + 1].mY <= P.mY))) { // a downward crossing
														  // compute  the actual edge-ray intersect x-coordinate
			float vt = (float)(P.mY - V[i].mY) / (V[i + 1].mY - V[i].mY);
			if (P.mX <  V[i].mX + vt * (V[i + 1].mX - V[i].mX)) // P.x < intersect
				++cn;   // a valid crossing of y=P.y right of P.x
		}
	}
	return (cn & 1);    // 0 if even (out), and 1 if  odd (in)

}

inline bool PointInPolygon(USVec2D point, NavPolygon polygon) {
	vector<USVec2D> points = polygon.mVerts;
	int i, j, nvert = points.size();
	bool c = false;

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((points[i].mY >= point.mY) != (points[j].mY >= point.mY)) &&
			(point.mX <= (points[j].mX - points[i].mX) * (point.mY - points[i].mY) / (points[j].mY - points[i].mY) + points[i].mX)
			)
			c = !c;
	}

	return c;
}

inline USVec2D compute2DPolygonCentroid(const USVec2D* vertices, int vertexCount)
{
	USVec2D centroid = { 0, 0 };
	double signedArea = 0.0;
	double x0 = 0.0; // Current vertex X
	double y0 = 0.0; // Current vertex Y
	double x1 = 0.0; // Next vertex X
	double y1 = 0.0; // Next vertex Y
	double a = 0.0;  // Partial signed area

					 // For all vertices except last
	int i = 0;
	for (i = 0; i<vertexCount - 1; ++i)
	{
		x0 = vertices[i].mX;
		y0 = vertices[i].mY;
		x1 = vertices[i + 1].mX;
		y1 = vertices[i + 1].mY;
		a = x0 * y1 - x1 * y0;
		signedArea += a;
		centroid.mX += (x0 + x1)*a;
		centroid.mY += (y0 + y1)*a;
	}

	// Do last vertex separately to avoid performing an expensive
	// modulus operation in each iteration.
	x0 = vertices[i].mX;
	y0 = vertices[i].mY;
	x1 = vertices[0].mX;
	y1 = vertices[0].mY;
	a = x0 * y1 - x1 * y0;
	signedArea += a;
	centroid.mX += (x0 + x1)*a;
	centroid.mY += (y0 + y1)*a;

	signedArea *= 0.5;
	centroid.mX /= (6.0*signedArea);
	centroid.mY /= (6.0*signedArea);

	return centroid;
}

#endif