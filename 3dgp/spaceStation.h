/*********************************************************************************
3DGL Space Station Class created by Jarek Francik for Kingston University students
Partially based on http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
Copyright (C) 2020 Jarek Francik, Kingston University, London, UK
*********************************************************************************/

#ifndef __spaceStation_h_
#define __spaceStation_h_

#include "GL/3dgl.h"
#include "glm/glm.hpp"

#include <vector>

class CSpaceStation
{
	std::vector<_3dgl::C3dglModel> models;
public:
	CSpaceStation()  { }

	bool loadModels(const char* pDefTexPath);
	void loadMaterials(const char* pDefTexPath);

	// rendering
	void renderSegment(glm::mat4 matrix);				// render the entire model
	void renderIntersection(glm::mat4 matrix);			// render the entire model
	void renderEnd(glm::mat4 matrix);					// render the entire model
};

#endif