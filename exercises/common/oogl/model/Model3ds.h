/*
Program name:Combat of Death
Description header file: this class loads 3ds models
Copyright (C) 2005  Hylke Donker

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef MODEL3DS_H_
#define MODEL3DS_H_

#include <lib3ds.h>

#include <oogl/Texture.h>
#include <oogl/Model.h>

#include <vector>

namespace oogl {
namespace model {

class Model3ds : public Model {
public:
	Model3ds(std::string fileName);
	~Model3ds();

	virtual void render();

private:
	Lib3dsFile *file;
	float currentFrame;
	std::vector<oogl::Texture*> textures;

	void loadFile();
	void renderNode(Lib3dsNode *node);
	void renderMeshNode(Lib3dsNode *node);
	void renderMeshImpl(Lib3dsMesh *mesh);

	void dump();

	bool hasSingleMaterial(Lib3dsMesh *mesh);
	oogl::Texture* applyMaterial(Lib3dsMaterial *material);
	std::string getDirectory(const std::string& fileName);
};

}}

#endif /* MODEL3DS_H_ */
