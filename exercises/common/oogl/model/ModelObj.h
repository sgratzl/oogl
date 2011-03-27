/*
 * ModelObj.h
 *
 *  Created on: 26.03.2011
 *      Author: sam
 */

#ifndef MODELOBJ_H_
#define MODELOBJ_H_

#include <glm/glm.h>

#include <oogl/Model.h>

#include <oogl/DisplayList.h>

namespace oogl {
namespace model {

class ModelObj : public Model {
public:
	ModelObj(std::string fileName, Model::LoadOptions options);
	virtual ~ModelObj();

	virtual void render(RenderOptions options = RENDER_NORMAL);

private:
	GLMmodel* model;
	oogl::DisplayList *displayList;

	void loadFile();
	void dump();

};

}}

#endif /* MODELOBJ_H_ */
