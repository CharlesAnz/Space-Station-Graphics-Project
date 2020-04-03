#include <iostream>
#include "GL/glew.h"
#include "GL/3dgl.h"
#include "GL/glut.h"
#include "GL/freeglut_ext.h"

#include "SpaceStation.h"

// Include GLM core features
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <fstream>
#include <fstream>

#pragma comment (lib, "glew32.lib")

using namespace std;
using namespace _3dgl;
using namespace glm;

// Space Station model names
string names[] = { "Box1", "Floor1", "Floor2", "Floor5", "IntersectionSideFloor", "Light2", "Profile1", "ProfileEnd", "ProfileIntersection", "SideFloor1", "SideFloor2", "SideWall1", "TopWall4", "Wall1", "Wall12", "WallIntersection", "WallIntersection1", "WallIntersection2", "WallIntersection3" };
enum NAMES       {  Box1,   Floor1,   Floor2,   Floor5,   IntersectionSideFloor,   Light2,   Profile1,   ProfileEnd,   ProfileIntersection,   SideFloor1,   SideFloor2,   SideWall1,   TopWall4,   Wall1,   Wall12,   WallIntersection,   WallIntersection1,   WallIntersection2,   WallIntersection3  };

const int MODEL_MAX = sizeof(names) / sizeof(string);

///////////////////////////////////////////////////////////////////////////////////
// CSpaceStation

bool CSpaceStation::loadModels(const char* pDefTexPath)
{
	models.reserve(MODEL_MAX);
	for (int i = 0; i < MODEL_MAX; i++)
	{
		C3dglModel model;
		models.push_back(model);

		string path = pDefTexPath;
		if (path.back() != '/' && path.back() != '\\') path += "/";
		string fbx = path + names[i] + ".fbx";
		if (!models.back().load(fbx.c_str())) return false;
	}
	return true;
}

void CSpaceStation::loadMaterials(const char* pDefTexPath)
{
	string path = pDefTexPath;
	if (path.back() != '/' && path.back() != '\\') path += "/";
	for (int i = 0; i < MODEL_MAX; i++)
	{
		string albedo = path + names[i] + "_Albedo.png";
		string normal = path + names[i] + "_Normal.png";
		string AO = path + names[i] + "_AO.png";
		string emission = path + names[i] + "_Emission.png";

		ifstream falbedo(albedo.c_str());
		ifstream fnormal(normal.c_str());
		ifstream fAO(AO.c_str());
		ifstream femission(emission.c_str());

		C3dglModel::MESH* pMesh = models[i].getMesh(0);
		CMaterial* pMat = pMesh->createNewMaterial();
		pMat->create();
		if (falbedo.good()) pMat->loadTexture(GL_TEXTURE0, albedo.c_str());
		if (fnormal.good()) pMat->loadTexture(GL_TEXTURE1, normal.c_str());
		if (fAO.good()) pMat->loadTexture(GL_TEXTURE2, AO.c_str());
		if (femission.good()) pMat->loadTexture(GL_TEXTURE3, emission.c_str());
	}
}

void CSpaceStation::renderSegment(glm::mat4 m)
{
	models[Profile1].render(m);
	models[Light2].render(m);

	m = translate(m, vec3(0, 0, 300.f));
	models[SideWall1].render(m);
	m = translate(m, vec3(0, 0, -300.f));
	m = rotate(m, radians(180.f), vec3(0.f, 1.f, 0.f));
	m = translate(m, vec3(0, 0, 300.f));
	models[SideWall1].render(m);
	m = translate(m, vec3(0, 0, -300.f));
	m = rotate(m, radians(180.f), vec3(1.f, 0.f, 0.f));
	m = translate(m, vec3(0, 0, 300.f));
	models[SideWall1].render(m);
	m = translate(m, vec3(0, 0, -300.f));
	m = rotate(m, radians(180.f), vec3(0.f, 1.f, 0.f));
	m = translate(m, vec3(0, 0, 300.f));
	models[SideWall1].render(m);
	m = translate(m, vec3(0, 0, -300.f));
	m = rotate(m, radians(180.f), vec3(1.f, 0.f, 0.f));

	models[Floor1].render(m);
	m = translate(m, vec3(0, 0, -150.f));
	models[Floor1].render(m);

	m = translate(m, vec3(0, 0, 450.f));
	models[SideFloor1].render(m);
	models[SideFloor2].render(m);
	models[TopWall4].render(m);

	m = translate(m, vec3(0, 0, 150.f));
	models[SideFloor1].render(m);
	models[SideFloor2].render(m);
	models[TopWall4].render(m);
	m = translate(m, vec3(0, 0, -450.f));

	m = rotate(m, radians(90.f), vec3(0.f, 1.f, 0.f));
	m = translate(m, vec3(900, -300, -600.f));
	models[Wall12].render(m);
	m = translate(m, vec3(-900, 300, 600.f));
	m = rotate(m, radians(180.f), vec3(0.f, 1.f, 0.f));
	m = translate(m, vec3(900, -300, -600.f));
	models[Wall12].render(m);
	m = translate(m, vec3(-900, 300, 600.f));
	m = rotate(m, radians(90.f), vec3(0.f, 1.f, 0.f));
}

void CSpaceStation::renderIntersection(glm::mat4 m)
{
	m = translate(m, vec3(88, 94.75, -367));
	models[WallIntersection].render(m);
	m = translate(m, vec3(-88, -94.75, 367));
	m = rotate(m, radians(180.f), vec3(0.f, 1.f, 0.f));
	m = translate(m, vec3(88, 94.75, -367));
	models[WallIntersection].render(m);
	m = translate(m, vec3(-88, -94.75, 367));
	m = rotate(m, radians(180.f), vec3(0.f, 1.f, 0.f));

	m = translate(m, vec3(0, 0, -600));

	models[ProfileIntersection].render(m);
	models[IntersectionSideFloor].render(m);
	m = translate(m, vec3(-198.5, 0, 0));
	models[IntersectionSideFloor].render(m);
	m = translate(m, vec3(198.5, 0, 0));

	models[WallIntersection1].render(m);
	models[WallIntersection2].render(m);
	models[WallIntersection3].render(m);
	m = translate(m, vec3(0, 0, 600));
	m = rotate(m, radians(90.f), vec3(0.f, 1.f, 0.f));
	m = translate(m, vec3(0, 0, -600));
	
	models[WallIntersection1].render(m);
	models[WallIntersection2].render(m);
	models[WallIntersection3].render(m);
	m = translate(m, vec3(0, 0, 600));
	m = rotate(m, radians(90.f), vec3(0.f, 1.f, 0.f));
	m = translate(m, vec3(0, 0, -600));

	models[WallIntersection1].render(m);
	models[WallIntersection2].render(m);
	models[WallIntersection3].render(m);
	m = translate(m, vec3(0, 0, 600));
	m = rotate(m, radians(90.f), vec3(0.f, 1.f, 0.f));
	m = translate(m, vec3(0, 0, -600));

	models[WallIntersection1].render(m);
	models[WallIntersection2].render(m);
	models[WallIntersection3].render(m);
	m = translate(m, vec3(0, 0, 600));
	m = rotate(m, radians(90.f), vec3(0.f, 1.f, 0.f));
	m = translate(m, vec3(0, 0, -600));


	m = translate(m, vec3(0, 0, 675.f));
	models[Floor2].render(m);
	models[Floor5].render(m);
	m = translate(m, vec3(0, 0, 225.f));
	models[Floor5].render(m);

	models[TopWall4].render(m);
	m = translate(m, vec3(0, 0, 150.f));
	models[TopWall4].render(m);
}

void CSpaceStation::renderEnd(glm::mat4 m)
{
	m = translate(m, vec3(600, 0, -600));
	models[ProfileEnd].render(m);
	m = translate(m, vec3(-600, 0, 600));

	m = rotate(m, radians(180.f), vec3(0.f, 1.f, 0.f));
	m = scale(m, vec3(1.f, 1.f, 0.5f));
	m = translate(m, vec3(-262, 60, -900));
	models[Box1].render(m);
}

