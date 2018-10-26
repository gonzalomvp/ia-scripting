#include <stdafx.h>
#include <tinyxml.h>
#include "params.h"

bool ReadParams(const char* filename, Params& params)
{
    TiXmlDocument doc(filename);
    if (!doc.LoadFile())
    {
        fprintf(stderr, "Couldn't read params from %s", filename);
        return false;
    }

    TiXmlHandle hDoc(&doc);

    TiXmlElement* pElem;
    pElem = hDoc.FirstChildElement().Element();
    if (!pElem)
    {
        fprintf(stderr, "Invalid format for %s", filename);
        return false;
    }

    TiXmlHandle hRoot(pElem);
    TiXmlHandle hParams = hRoot.FirstChildElement("params");

    //TiXmlElement* paramElem = hParams.FirstChild().Element();
    //for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement())
    //{
    //    const char* paramName = paramElem->Value();
    //    if (!strcmp(paramName, "max_velocity"))
    //    {
    //        const char* elemText = paramElem->GetText();
    //        params.max_velocity = static_cast<float>(atof(elemText));
    //    }
    //}

    TiXmlElement* paramElem = hParams.FirstChildElement("max_velocity").Element();
    if (paramElem)
        paramElem->Attribute("value", &params.max_velocity);

    paramElem = hParams.FirstChildElement("max_acceleration").Element();
    if (paramElem)
        paramElem->Attribute("value", &params.max_acceleration);

    paramElem = hParams.FirstChildElement("dest_radius").Element();
    if (paramElem)
        paramElem->Attribute("value", &params.dest_radius);

	paramElem = hParams.FirstChildElement("arrive_radius").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.arrive_radius);

    paramElem = hParams.FirstChildElement("targetPosition").Element();
    if (paramElem)
    {
        paramElem->Attribute("x", &params.targetPosition.mX);
        paramElem->Attribute("y", &params.targetPosition.mY);
    }

	paramElem = hParams.FirstChildElement("max_angular_velocity").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.max_angular_velocity);

	paramElem = hParams.FirstChildElement("max_angular_acceleration").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.max_angular_acceleration);

	paramElem = hParams.FirstChildElement("angular_dest_radius").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.angular_dest_radius);

	paramElem = hParams.FirstChildElement("angular_arrive_radius").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.angular_arrive_radius);

	paramElem = hParams.FirstChildElement("targetRotation").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.targetRotation);

	paramElem = hParams.FirstChildElement("look_ahead").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.look_ahead);

	paramElem = hParams.FirstChildElement("time_ahead").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.time_ahead);

	paramElem = hParams.FirstChildElement("enemy_speed").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.enemy_speed);

	paramElem = hParams.FirstChildElement("enemy_minPosition").Element();
	if (paramElem)
	{
		paramElem->Attribute("x", &params.enemy_minPosition.mX);
		paramElem->Attribute("y", &params.enemy_minPosition.mY);
	}

	paramElem = hParams.FirstChildElement("enemy_maxPosition").Element();
	if (paramElem)
	{
		paramElem->Attribute("x", &params.enemy_maxPosition.mX);
		paramElem->Attribute("y", &params.enemy_maxPosition.mY);
	}

	paramElem = hParams.FirstChildElement("char_radius").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.char_radius);

    return true;
}

bool ReadPath(const char* filename, std::vector<USVec2D>& path)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		fprintf(stderr, "Couldn't read params from %s", filename);
		return false;
	}

	TiXmlHandle hDoc(&doc);

	TiXmlElement* pElem;
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		fprintf(stderr, "Invalid format for %s", filename);
		return false;
	}

	TiXmlHandle hRoot(pElem);
	TiXmlHandle hParams = hRoot.FirstChildElement("points");

	TiXmlElement* paramElem = hParams.FirstChild().Element();
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement())
	{
	    const char* paramName = paramElem->Value();
	    if (!strcmp(paramName, "point"))
	    {
			USVec2D point;
			paramElem->Attribute("x", &point.mX);
			paramElem->Attribute("y", &point.mY);
			path.push_back(point);
	    }
	}
	return true;
}

bool ReadObstacles(const char* filename, std::vector<USVec3D>& obstacles)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		fprintf(stderr, "Couldn't read params from %s", filename);
		return false;
	}

	TiXmlHandle hDoc(&doc);

	TiXmlElement* pElem;
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		fprintf(stderr, "Invalid format for %s", filename);
		return false;
	}

	TiXmlHandle hRoot(pElem);
	TiXmlHandle hParams = hRoot.FirstChildElement("obstacles");

	TiXmlElement* paramElem = hParams.FirstChild().Element();
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement())
	{
		const char* paramName = paramElem->Value();
		if (!strcmp(paramName, "obstacle"))
		{
			USVec3D obstacle;
			paramElem->Attribute("x", &obstacle.mX);
			paramElem->Attribute("y", &obstacle.mY);
			paramElem->Attribute("r", &obstacle.mZ);
			obstacles.push_back(obstacle);
		}
	}
	return true;
}

bool ReadNavmesh(const char* filename, std::vector<NavPolygon>& polygons)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		fprintf(stderr, "Couldn't read params from %s", filename);
		return false;
	}

	TiXmlHandle hDoc(&doc);

	TiXmlElement* pElem;
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		fprintf(stderr, "Invalid format for %s", filename);
		return false;
	}

	TiXmlHandle hRoot(pElem);
	TiXmlHandle hParams = hRoot.FirstChildElement("polygons").FirstChildElement("polygon");

	TiXmlElement* poligonElem = hParams.Element();
	for (poligonElem; poligonElem; poligonElem = poligonElem->NextSiblingElement()) {
		NavPolygon polygon;
		TiXmlElement* pointElem = poligonElem->FirstChildElement("point");
		for (pointElem; pointElem; pointElem = pointElem->NextSiblingElement()) {
			const char* paramName = pointElem->Value();
			if (!strcmp(paramName, "point")) {
				USVec2D point;
				pointElem->Attribute("x", &point.mX);
				pointElem->Attribute("y", &point.mY);
				polygon.mVerts.push_back(point);
			}
		}

		polygons.push_back(polygon);
		
	}

	hParams = hRoot.FirstChildElement("links").FirstChildElement("link");
	TiXmlElement* linkElem = hParams.Element();
	for (linkElem; linkElem; linkElem = linkElem->NextSiblingElement()) {
		int startPolygon, startEdgestart, startEdgeend, endPolygon, endEdgestart, endEdgeend;

		TiXmlElement* startElem = linkElem->FirstChildElement("start");
		startElem->Attribute("polygon", &startPolygon);
		startElem->Attribute("edgestart", &startEdgestart);
		startElem->Attribute("edgeend", &startEdgeend);

		TiXmlElement* endElem = linkElem->FirstChildElement("end");
		endElem->Attribute("polygon", &endPolygon);
		endElem->Attribute("edgestart", &endEdgestart);
		endElem->Attribute("edgeend", &endEdgeend);

		NavPolygon::Edge edge;
		edge.mVerts[0] = startEdgestart;
		edge.mVerts[1] = startEdgeend;
		edge.mNeighbour = &polygons[startPolygon];
		polygons[endPolygon].mEdges.push_back(edge);

		edge.mVerts[0] = endEdgestart;
		edge.mVerts[1] = endEdgeend;
		edge.mNeighbour = &polygons[endPolygon];
		polygons[startPolygon].mEdges.push_back(edge);
	}

	return true;
}