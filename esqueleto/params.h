#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "pathfinding/map_node.h"
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

bool ReadParams(const char* filename, Params& params);
bool ReadPath(const char* filename, std::vector<USVec2D>& path);
bool ReadObstacles(const char* filename, std::vector<USVec3D>& obstacles);
bool ReadGrid(const char* filename, std::vector<MapNode*>& cells);
bool ReadNavmesh(const char* filename, std::vector<MapNode*>& polygons);

#endif