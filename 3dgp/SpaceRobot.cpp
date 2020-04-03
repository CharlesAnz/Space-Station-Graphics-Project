#include <iostream>
#include "GL/glew.h"
#include "GL/3dgl.h"
#include "GL/glut.h"
#include "GL/freeglut_ext.h"

#include "SpaceRobot.h"

// Include GLM core features
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <fstream>
#include <fstream>

#pragma comment (lib, "glew32.lib")

using namespace std;
using namespace _3dgl;
using namespace glm;

///////////////////////////////////////////////////////////////////////////////////
// CSpaceRobot

void CSpaceRobot::loadAnimations(const char* pFile)
{
	ifstream myfile;
	myfile.open("models\\robot\\frspbt.anim");

	FRAME frame;
	char comma;
	int ctrl = 1;
	float t;

	m_frames.reserve(10);

	while (ctrl)
	{
		myfile >> frame.name >> frame.id >> comma >> ctrl >> comma >> frame.fr >> comma >> t >> comma;
		for (int i = 0; i < 6; i++)
		{
			myfile >> frame.m[i].x >> comma >> frame.m[i].y >> comma >> frame.m[i].z;
			if (i < 5) myfile >> comma;
		}

		if (frame.id >= m_frames.size())
		{
			vector<FRAME> v;
			v.reserve(120);
			m_frames.push_back(v);
		}
		m_frames.back().push_back(frame);
	}
	myfile.close();
	m_anim = -1; 
}

void  CSpaceRobot::render(glm::mat4 m)
{
	m = translate(m, vec3(0, -50, -0));
	m = scale(m, vec3(12, 12, 12));
	m = rotate(m, radians(180.f), vec3(0.f, 1.f, 0.f));
	if (m_anim < 0)
		C3dglModel::render(m);
	else
	{
		if (m_t == 0) { m_t = glutGet(GLUT_ELAPSED_TIME); m_lastFr = 0; }
		FRAME fr = m_frames[m_anim][((glutGet(GLUT_ELAPSED_TIME) - m_t) * 60 / 1000) % m_frames[m_anim].size()];
		if (!(m_anim & 1) && fr.fr < (unsigned)m_lastFr)
		{
			m_anim++;
			m_t = glutGet(GLUT_ELAPSED_TIME);
			fr = m_frames[m_anim][((glutGet(GLUT_ELAPSED_TIME) - m_t) * 60 / 1000) % m_frames[m_anim].size()];
		}
		m_lastFr = fr.fr;
		int k = 1;
		aiVector3D vecAnkle = getMesh(2)->getCentre(); vecAnkle.x = 0; vecAnkle.y = -2.67027f;
		aiVector3D vecHip = getMesh(6)->getCentre();
		C3dglModel::render(1, m);
		m = translate(m, vec3(vecAnkle.x, vecAnkle.y, vecAnkle.z));
		m = rotate(m, -radians(fr.m[- --k].z), vec3(0.f, 0.f, 1.f));
		m = rotate(m, -radians(fr.m[-k].x), vec3(1.f, 0.f, 0.f));
		m = translate(m, vec3(-vecAnkle.x, -vecAnkle.y, -vecAnkle.z));
		C3dglModel::render(2, m);	// ankle
		C3dglModel::render(3, m);
		m = rotate(m, -radians(fr.m[- --k].z), vec3(0.f, 0.f, 1.f));
		m = rotate(m, -radians(fr.m[-k].x), vec3(1.f, 0.f, 0.f));
		C3dglModel::render(4, m);	// knee
		C3dglModel::render(5, m);
		m = translate(m, vec3(vecHip.x, vecHip.y, vecHip.z));
		m = rotate(m, -radians(fr.m[- --k].z), vec3(0.f, 0.f, 1.f));
		m = rotate(m, -radians(fr.m[-k].x), vec3(1.f, 0.f, 0.f));
		m = translate(m, vec3(-vecHip.x, -vecHip.y, -vecHip.z));
		C3dglModel::render(6, m);	// hip
		C3dglModel::render(7, m);
		C3dglModel::render(0, m);
		m = translate(m, vec3(vecHip.x, vecHip.y, vecHip.z));
		m = rotate(m, -radians(fr.m[- --k].z), vec3(0.f, 0.f, 1.f));
		m = rotate(m, -radians(fr.m[-k].x), vec3(1.f, 0.f, 0.f));
		m = translate(m, vec3(-vecHip.x, -vecHip.y, -vecHip.z));
		C3dglModel::render(13, m);	// hip
		C3dglModel::render(12, m);
		m = rotate(m, -radians(fr.m[- --k].z), vec3(0.f, 0.f, 1.f));
		m = rotate(m, -radians(fr.m[-k].x), vec3(1.f, 0.f, 0.f));
		C3dglModel::render(11, m);	// knee
		C3dglModel::render(10, m);
		m = translate(m, vec3(vecAnkle.x, vecAnkle.y, vecAnkle.z));
		m = rotate(m, -radians(fr.m[- --k].z), vec3(0.f, 0.f, 1.f));
		m = rotate(m, -radians(fr.m[-k].x), vec3(1.f, 0.f, 0.f));
		m = translate(m, vec3(-vecAnkle.x, -vecAnkle.y, -vecAnkle.z));
		C3dglModel::render(9, m);	// ankle
		C3dglModel::render(8, m);
	}
}

void CSpaceRobot::AnimateIdle()
{
	m_anim = 0 + (m_anim & 4) + ((m_anim & 4) >> 1) - ((m_anim & 4) >> 2)* ((m_anim & 2) >> 1) * 6; m_t = 0;
}

void CSpaceRobot::AnimateActivated()
{
	m_anim = 2 + (m_anim & 4) + ((m_anim & 4) >> 1) - ((m_anim & 4) >> 2)* ((m_anim & 2) >> 1) * 6; m_t = 0;
}

void CSpaceRobot::AnimateAlarmed()
{
	m_anim = 4; m_t = 0;
}
