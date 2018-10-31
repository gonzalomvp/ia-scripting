#include <stdafx.h>
#include "utils.h"
#include "pathfinding/map_node.h"

void closestPointToSegments(const USVec2D& currentPosition, const std::vector<USVec2D>& path, USVec2D& closestPoint, int& closestSegment) {
	closestSegment = -1;
	float minDistance = 99999999999.0f;
	for (size_t i = 0; i + 1 < path.size(); ++i) {
		USVec2D point = closestPointToSegment(currentPosition, path[i], path[i + 1]);
		float distance = (point - currentPosition).Length();
		if (distance < minDistance) {
			minDistance = distance;
			closestSegment = i;
			closestPoint = point;
		}
	}
}

USVec2D closestPointToSegment(const USVec2D& currentPosition, const USVec2D& segmentStart, const USVec2D& segmentEnd) {
	USVec2D closestPoint;

	USVec2D np = (segmentEnd - segmentStart);
	np.NormSafe();
	USVec2D r0 = currentPosition - segmentStart;
	float proj = r0.Dot(np);

	if (proj > (segmentEnd - segmentStart).Length()) {
		closestPoint = segmentEnd;
	}
	else if (proj < 0) {
		closestPoint = segmentStart;
	}
	else {
		closestPoint = segmentStart + (np * proj);
	}

	return closestPoint;
}

USVec2D lookAheadInPath(const USVec2D& point, const std::vector<USVec2D>& path, float lookAhead) {
	USVec2D lookAheadPoint = path[path.size() - 1];
	float totalAhead = 0.0f;
	USVec2D closestPoint;
	int segmentIndex;

	closestPointToSegments(point, path, closestPoint, segmentIndex);

	USVec2D segmentStart = point;
	USVec2D segmentEnd = path[segmentIndex + 1];
	float segmentLength = (segmentEnd - point).Length();

	while (totalAhead + segmentLength < lookAhead) {
		++segmentIndex;
		if (segmentIndex + 1 < static_cast<int>(path.size())) {
			totalAhead += segmentLength;
			segmentStart = path[segmentIndex];
			segmentEnd = path[segmentIndex + 1];
			segmentLength = (segmentEnd - segmentStart).Length();
		}
		else {
			break;
		}
	}

	if (totalAhead + segmentLength >= lookAhead) {
		USVec2D segmentDir = segmentEnd - segmentStart;
		segmentDir.NormSafe();
		lookAheadPoint = segmentStart + segmentDir * (lookAhead - totalAhead);
	}

	return lookAheadPoint;
}

bool getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y,
	float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;
	float s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);
	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		if (i_x != NULL)
			*i_x = p0_x + (t * s1_x);
		if (i_y != NULL)
			*i_y = p0_y + (t * s1_y);
		return true;
	}
	return false; // No collision
}

bool isPointInPolygon(const USVec2D& point, const NavPolygon& polygon) {
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
