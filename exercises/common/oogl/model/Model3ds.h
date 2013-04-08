/*
 * Model3ds.h
 *
 *  Created on: 26.03.2011
 *      Author: sam
 */



#pragma once

#include <lib3ds.h>

#include <oogl/Texture.h>
#include <oogl/Model.h>

#include <vector>

namespace oogl {
namespace model {

class Model3ds : public Model {
public:
	Model3ds(std::string fileName, Model::LoadOptions options);
	~Model3ds();

	virtual void render(RenderOptions options = 0);

private:
	Lib3dsFile *file;
	float currentFrame;
	std::vector<oogl::Texture*> textures;

	void loadFile();
	void renderNode(Lib3dsNode *node, RenderOptions options);
	void renderMeshNode(Lib3dsNode *node, RenderOptions options);
	void renderMeshImpl(Lib3dsMeshInstanceNode *node, Lib3dsMesh *mesh, RenderOptions options);

	void dump();

	oogl::Texture* applyMaterial(Lib3dsMaterial *material, RenderOptions options);
	std::string getDirectory(const std::string& fileName);
};

}}

