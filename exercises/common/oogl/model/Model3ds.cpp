/*
Program name:Combat of Death
Description source file: this class loads 3ds models
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

#include <oogl/model/Model3ds.h>
#include <oogl/Texture.h>
#include <oogl/gl_error.h>

#include <utils/log.h>

#include <cstring>
#include <stdexcept>
#include <algorithm>

#include <glm/glm_ostream.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace oogl {
namespace model {

Model3ds::Model3ds(std::string fileName, Model::LoadOptions options) :
	Model(fileName, options), currentFrame(0) {
	loadFile();
}

Model3ds::~Model3ds() {
	LOG_DEBUG << "free model " << fileName << std::endl;
	if (file)
		lib3ds_file_free(file);

	for(std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		delete *it;
	textures.clear();
}

inline bool MaterialComparer(const Lib3dsFace& a, const Lib3dsFace& b) {
	return a.material < b.material;
}


void Model3ds::loadFile() {
	LOG_DEBUG << "load 3ds file" << fileName << std::endl;
	file = lib3ds_file_open(fileName.c_str());
	if (!file) {
		LOG_ERROR << "invalid model " << fileName << std::endl;
		throw std::runtime_error("invalid model: "+fileName);
		return;
	}

	if (!file->nodes) {
		LOG_DEBUG << "generate dummy nodes" << fileName << std::endl;
		for (int i = 0; i < file->nmeshes; ++i) {
			Lib3dsMesh *mesh = file->meshes[i];
			Lib3dsNode *node = lib3ds_node_new(LIB3DS_NODE_MESH_INSTANCE);
#if WIN32
			strcpy_s(node->name, 64, mesh->name);
#else
			strcpy(node->name, mesh->name);
#endif
			lib3ds_file_insert_node(file, node, NULL);
		}
	}

	static float bmin[3], bmax[3];
	lib3ds_file_bounding_box_of_nodes(file, 1, 0, 0, bmin, bmax, NULL);

	boundingBox.min = glm::vec3(bmin[0],bmin[1],bmin[2]);
	boundingBox.max = glm::vec3(bmax[0],bmax[1],bmax[2]);

	lib3ds_file_eval(file, 0.0f); // set current frame to 0

	dump();

	//preload all textures, to avoid lazy initialization of DevIL during rendering
	for(int i = 0; i < file->nmaterials; ++i) {
		Lib3dsMaterial *material = file->materials[i];
		if (material->texture1_map.name[0] != '\0') {
			//has texture
			Lib3dsTextureMap *tex = &material->texture1_map;
			LOG_DEBUG << "found texture " << tex->name << std::endl;
			if (tex->user_id == 0) { /*no texture yet*/
				oogl::Texture* texture = oogl::loadTexture(getDirectory(fileName) + std::string(tex->name));
				textures.push_back(texture);
				tex->user_id = (int)textures.size(); //index + 1 to have 0 as invalid value
			}
		}
	}

	//sort faces according to their material index for better rendering
	for(int i = 0; i < file->nmeshes; ++i) {
		Lib3dsMesh *mesh = file->meshes[i];
		std::stable_sort(mesh->faces+0,mesh->faces+(mesh->nfaces), MaterialComparer);
	}

	LOG_DEBUG << "loaded " << fileName << std::endl;
}

void Model3ds::render(RenderOptions options) {
	LOG_DEBUG << "render " << fileName << std::endl;

	//backup set attributes
	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_POLYGON_BIT | GL_TEXTURE_BIT);

	if(!(options & RENDER_NO_CULL_FACE)) {
		glEnable( GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	for (Lib3dsNode *node = file->nodes; node != NULL; node = node->next) {
		renderNode(node, options);
	}

	currentFrame++;
	if (currentFrame > file->frames) //if the next frame doesn't exist, go to frame 0
		currentFrame = 0;
	lib3ds_file_eval(file, currentFrame); // set current frame

	LOG_GL_ERRORS();
	glPopAttrib();
}

void Model3ds::renderNode(Lib3dsNode *node, RenderOptions options) {
	assert(file != NULL);
	{
		for (Lib3dsNode *child = node->childs; child != NULL; child = child->next)
			renderNode(child, options); //render all child nodes of this node
	}

	switch (node->type) {
	case LIB3DS_NODE_MESH_INSTANCE:
		renderMeshNode(node, options);
		break;
	case LIB3DS_NODE_AMBIENT_COLOR:
		LOG_DEBUG << "can' render node type: ambient color" << std::endl;
		break;
	case LIB3DS_NODE_CAMERA:
		LOG_DEBUG << "can' render node type: camera" << std::endl;
		break;
	case LIB3DS_NODE_CAMERA_TARGET:
		LOG_DEBUG << "can' render node type: camera target" << std::endl;
		break;
	case LIB3DS_NODE_OMNILIGHT:
		LOG_DEBUG << "can' render node type: omnilight" << std::endl;
		break;
	case LIB3DS_NODE_SPOTLIGHT:
		LOG_DEBUG << "can' render node type: spotlight" << std::endl;
		break;
	case LIB3DS_NODE_SPOTLIGHT_TARGET:
		LOG_DEBUG << "can' render node type: spotlight target" << std::endl;
		break;
	default:
		LOG_WARN << "can' render node type: unknown" << std::endl;
	}
}

void Model3ds::renderMeshNode(Lib3dsNode *node, RenderOptions options) {
	//LOG_DEBUG << "render mesh node " << node->name << std::endl;

	Lib3dsMeshInstanceNode *n = (Lib3dsMeshInstanceNode*) node;

	if (strcmp(node->name, "$$$DUMMY") == 0) {
		return;
	}

	int index = lib3ds_file_mesh_by_name(file, n->instance_name);
	if (index < 0)
		index = lib3ds_file_mesh_by_name(file, node->name);
	if (index < 0) {
		LOG_WARN << "can find mesh node " << node->name << std::endl;
		return;
	}

	Lib3dsMesh *mesh = file->meshes[index];

	if(options & RENDER_NO_DISLAYLIST) {
		renderMeshImpl(n, mesh, options);
	} else if (!mesh->user_id) {
		assert(mesh);

		mesh->user_id = glGenLists(1);
		glNewList(mesh->user_id, GL_COMPILE_AND_EXECUTE);

		renderMeshImpl(n, mesh, options);

		glEndList();
	} else {
		glCallList(mesh->user_id);
	}
	LOG_GL_ERRORS();
}

oogl::Texture* Model3ds::applyMaterial(Lib3dsMaterial *material, RenderOptions options) {
	if(!(options & RENDER_NO_CULL_FACE)) {
		if (material->two_sided)
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);
	}

	LOG_DEBUG << "apply material: " << material->name << std::endl;

	glm::vec4 ambient(material->ambient[0],material->ambient[1],material->ambient[2], 1-material->transparency);
	glm::vec4 diffuse(material->diffuse[0],material->diffuse[1],material->diffuse[2], 1-material->transparency);
	glm::vec4 specular(material->specular[0],material->specular[1],material->specular[2], 1-material->transparency);
	float shininess = glm::clamp(glm::pow(2.f, 10.0f * material->shininess),0.f,128.f);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(ambient));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(diffuse));
	LOG_DEBUG << "ambient " << ambient << std::endl;
	LOG_DEBUG << "diffuse " << diffuse << std::endl;
	LOG_DEBUG << "specular " << specular << std::endl;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specular));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	if (!(options & RENDER_NO_TEXTURES) && material->texture1_map.name[0] != '\0') {
		oogl::Texture* texture;
		//has texture
		Lib3dsTextureMap *tex = &material->texture1_map;
		LOG_DEBUG << "found texture " << tex->name << std::endl;
		if (tex->user_id == 0) { /*no texture yet*/
			LOG_ERROR << "here but shound't be there" << std::endl;
			texture = oogl::loadTexture(getDirectory(fileName) + std::string(tex->name));
			textures.push_back(texture);
			tex->user_id = (int)textures.size(); //index + 1 to have 0 as invalid value
		} else {
			LOG_DEBUG << "reuse texture " << tex->user_id-1 << std::endl;
			texture = textures.at((tex->user_id-1));
		}
		return texture;
	}
	return NULL;
}

std::string Model3ds::getDirectory(const std::string& fileName) {
	if(fileName.find_last_of("/") != std::string::npos)
		return fileName.substr(0,fileName.find_last_of("/")+1);
	else if(fileName.find_last_of("\\") != std::string::npos)
		return fileName.substr(0,fileName.find_last_of("\\")+1);
	return "";
}

void Model3ds::renderMeshImpl(Lib3dsMeshInstanceNode *node, Lib3dsMesh *mesh, RenderOptions options) {
	LOG_DEBUG << "render mesh impl " << node->base.name << std::endl;
	glPushMatrix();

	LOG_DEBUG << fileName << " matrix " << std::endl
		<< node->base.matrix[0][0] << " " << node->base.matrix[0][1] << " " << node->base.matrix[0][2] << " " << node->base.matrix[0][3] << std::endl
		<< node->base.matrix[1][0] << " " << node->base.matrix[1][1] << " " << node->base.matrix[1][2] << " " << node->base.matrix[1][3] << std::endl
		<< node->base.matrix[2][0] << " " << node->base.matrix[2][1] << " " << node->base.matrix[2][2] << " " << node->base.matrix[2][3] << std::endl
		<< node->base.matrix[3][0] << " " << node->base.matrix[3][1] << " " << node->base.matrix[3][2] << " " << node->base.matrix[3][3] << std::endl;

	glMultMatrixf(&node->base.matrix[0][0]);
	LOG_DEBUG << fileName << " pivot " << node->pivot[0] << " " <<node->pivot[1] << " " << node->pivot[2] << std::endl;
	glTranslatef(-node->pivot[0], -node->pivot[1], -node->pivot[2]);

	if(!(loadOptions & LOAD_NO_NORMALIZATION)) {
		BoundingSphere bsphere = getBoundingSphere();
		float normalize = 1.f/bsphere.radius;
		if(loadOptions & LOAD_NORMALIZE_TWO)
			normalize = normalize * 2;
		LOG_DEBUG << fileName << " normalize factor " << normalize << std::endl;

		glScalef(normalize,normalize,normalize);
		glTranslatef(-bsphere.center.x,-bsphere.center.y,-bsphere.center.z);
	}

	//calculate vertex normals
	float (*normalL)[3] = (float(*)[3]) malloc(3 * 3 * sizeof(float) * mesh->nfaces);
	lib3ds_mesh_calculate_vertex_normals(mesh, normalL);

	oogl::Texture* texture = NULL;

	if(options & RENDER_NO_MATERIALS) {
		glBegin( GL_TRIANGLES);
		{
			for(int p = 0; p < mesh->nfaces; ++p) {
				Lib3dsFace *face = &(mesh->faces[p]);
				for (int i = 0; i < 3; ++i) {
					glNormal3fv(normalL[3 * p + i]);
					glVertex3fv(mesh->vertices[face->index[i]]);
				}
			}
		}
		glEnd();
	} else { //with materials
		LOG_DEBUG << "multi material -> ordinary slow rendering" << std::endl;
		int actMaterialIndex = -1;
		bool began = false;
		oogl::Texture* actTexture = NULL;

		for(int p = 0; p < mesh->nfaces; ++p) {
			Lib3dsFace *face = &(mesh->faces[p]);
			if(face->material != actMaterialIndex) { //material/texture change
				if(began) {
					glEnd();
					began = false;
				}
				if(actTexture != NULL)
					actTexture->unbind();
				actMaterialIndex = face->material;
				actTexture = NULL;

				if(actMaterialIndex >= 0) { //has material apply and save
					Lib3dsMaterial *mat = file->materials[actMaterialIndex];
					actTexture = applyMaterial(mat, options);
					if(actTexture != NULL)
						actTexture->bind();
				}
			}
			if(!began) {
				glBegin( GL_TRIANGLES);
				began = true;
			}
			{
				if(actTexture != NULL) {
					for (int i = 0; i < 3; ++i) {
						glNormal3fv(normalL[3 * p + i]);
						glTexCoord2f(mesh->texcos[face->index[i]][0], mesh->texcos[face->index[i]][1]);
						glVertex3fv(mesh->vertices[face->index[i]]);
					}
				} else {
					for (int i = 0; i < 3; ++i) {
						glNormal3fv(normalL[3 * p + i]);
						glVertex3fv(mesh->vertices[face->index[i]]);
					}
				}
			}
		}
		if(began)
			glEnd();
		if(actTexture != NULL)
			actTexture->unbind();
	}

	free(normalL);

	glPopMatrix();
}

void Model3ds::dump() {
	if(!LOG_IS_DEBUG_ENABLED) return;

	BoundingSphere bs = getBoundingSphere();
	LOG_DEBUG << fileName << std::endl
		<< "\t" << "name: " << file->name << std::endl
		<< "\t" << "boundingBox: " << boundingBox.min << " - " << boundingBox.max << std::endl
		<< "\t" << "boundingSphere: " << bs.center << " - " << bs.radius << std::endl
		<< "\t" << "num materials: " << file->nmaterials << std::endl
		<< "\t" << "num lights: " << file->nlights << std::endl
		<< "\t" << "num cameras: " << file->ncameras << std::endl
		<< "\t" << "num meshes: " << file->nmeshes << std::endl;
}

}
}
