#pragma once
#include "params.h"

void    closestPointToSegments(const USVec2D& currentPosition, const std::vector<USVec2D>& path, USVec2D& closestPoint, int& closestSegment);
USVec2D closestPointToSegment (const USVec2D& currentPosition, const USVec2D& segmentStart, const USVec2D& segmentEnd);
USVec2D lookAheadInPath       (const USVec2D& point, const std::vector<USVec2D>& path, float lookAhead);