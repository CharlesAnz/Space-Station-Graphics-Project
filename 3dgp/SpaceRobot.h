/*********************************************************************************
3DGL Space Robot Class created by Jarek Francik for Kingston University students
Partially based on http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
Copyright (C) 2020 Jarek Francik, Kingston University, London, UK
*********************************************************************************/

#ifndef __spaceRobot_h_
#define __spaceRobot_h_

#include "GL/3dgl.h"
#include "glm/glm.hpp"

#include <vector>

class CSpaceRobot : public _3dgl::C3dglModel
{
	struct FRAME { std::string name; unsigned id, fr; glm::vec3 m[6]; };
	std::vector<std::vector<FRAME> > m_frames;
	int m_anim, m_lastFr;
	int m_t;

public:
	CSpaceRobot() : C3dglModel() { m_anim = 0; }

	void loadAnimations(const char* pFile);		// load the animation file
	void render(glm::mat4 m);					// render the animated model

	// START ANIMATIONS:
	void AnimateIdle();
	void AnimateActivated();
	void AnimateAlarmed();
};

#endif