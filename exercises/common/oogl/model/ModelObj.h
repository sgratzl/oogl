/*
 * ModelObj.h
 *
 *  Created on: 26.03.2011
 *      Author: sam
 */

#pragma once

#include <glm/glm.h>

#include <oogl/Model.h>

#include <oogl/DisplayList.h>

namespace oogl {
namespace model {

class ModelObj : public Model {
public:
	ModelObj(std::string fileName, Model::LoadOptions options);
	virtual ~ModelObj();

	virtual void render(RenderOptions options = 0);

private:
	GLMmodel* model;
	oogl::DisplayList *displayList;

	void loadFile();
	void dump();

};

}}

