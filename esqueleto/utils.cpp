#include <stdafx.h>
#include "utils.h"

void closestPointToSegments(const USVec2D& currentPosition, const std::vector<USVec2D>& path, USVec2D& closestPoint, int& closestSegment) {
	int closestSegmentIndex = -1;
	float minDistance = 99999999999;
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
		if (segmentIndex + 1 < path.size()) {
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