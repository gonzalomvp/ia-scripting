#pragma once

struct NavPolygon;

void    closestPointToSegments(const USVec2D& currentPosition, const std::vector<USVec2D>& path, USVec2D& closestPoint, int& closestSegment);
USVec2D closestPointToSegment (const USVec2D& currentPosition, const USVec2D& segmentStart, const USVec2D& segmentEnd);
USVec2D lookAheadInPath       (const USVec2D& point, const std::vector<USVec2D>& path, float lookAhead);
bool    getLineIntersection   (float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y);
bool    isPointInPolygon      (const USVec2D& point, const NavPolygon& polygon);