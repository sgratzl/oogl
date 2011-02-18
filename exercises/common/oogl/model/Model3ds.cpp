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

#include <utils/log.h>

#include <glm/glm_ostream.hpp>


namespace oogl {
namespace model {

Model3ds::Model3ds(std::string fileName) :
	Model(fileName), currentFrame(0) {
	loadFile();
}

Model3ds::~Model3ds() {
	if (file)
		lib3ds_file_free(file);
}

void Model3ds::loadFile() {
	LOG_DEBUG << "load 3ds file" << fileName << std::endl;
	file = lib3ds_file_open(fileName.c_str());
	if (!file) {
		LOG_ERROR << "invalid model " << fileName << std::endl;
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
	static float sx, sy, sz, size; /* bounding box dimensions */
	static float cx, cy, cz;  /* bounding box center */
	lib3ds_file_bounding_box_of_nodes(file, 1, 0, 0, bmin, bmax, NULL);

	boundingBox.min = glm::vec3(bmin[0],bmin[1],bmin[2]);
	boundingBox.max = glm::vec3(bmax[0],bmax[1],bmax[2]);

	lib3ds_file_eval(file, 0.0f); // set current frame to 0

	dump();

	LOG_DEBUG << "loaded " << fileName << std::endl;
}

void Model3ds::render() {
	LOG_DEBUG << "render " << fileName << std::endl;

	glEnable( GL_CULL_FACE);
	glCullFace( GL_BACK);

	for (Lib3dsNode *node = file->nodes; node != NULL; node = node->next) {
		renderNode(node);
	}

	currentFrame++;
	if (currentFrame > file->frames) //if the next frame doesn't exist, go to frame 0
		currentFrame = 0;
	lib3ds_file_eval(file, currentFrame); // set current frame
}

void Model3ds::renderNode(Lib3dsNode *node) {
	assert(file != NULL);
	{
		for (Lib3dsNode *child = node->childs; child != NULL; child = child->next)
			renderNode(child); //render all child nodes of this note
	}

	switch (node->type) {
	case LIB3DS_NODE_MESH_INSTANCE:
		renderMeshNode(node);
		break;
	case LIB3DS_NODE_AMBIENT_COLOR:
		LOG_DEBUG << "can' render node type ambient color" << std::endl;
		break;
	case LIB3DS_NODE_CAMERA:
		LOG_DEBUG << "can' render node type camera" << std::endl;
		break;
	case LIB3DS_NODE_CAMERA_TARGET:
		LOG_DEBUG << "can' render node type camera target" << std::endl;
		break;
	case LIB3DS_NODE_OMNILIGHT:
		LOG_DEBUG << "can' render node type omnilight" << std::endl;
		break;
	case LIB3DS_NODE_SPOTLIGHT:
		LOG_DEBUG << "can' render node type spotlight" << std::endl;
		break;
	case LIB3DS_NODE_SPOTLIGHT_TARGET:
		LOG_DEBUG << "can' render node type spotlight target" << std::endl;
		break;
	default:
		LOG_WARN << "can' render node type unknown" << std::endl;
	}
}

void Model3ds::renderMeshNode(Lib3dsNode *node) {
	LOG_DEBUG << "render mesh node " << node->name << std::endl;

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

	if (!mesh->user_id) {
		assert(mesh);

		mesh->user_id = glGenLists(1);
		glNewList(mesh->user_id, GL_COMPILE);

		LOG_DEBUG << "render mesh impl " << node->name << std::endl;
		glPushMatrix();

		BoundingSphere bsphere = getBoundingSphere();
		float normalize = 2/bsphere.radius;

		LOG_DEBUG << fileName << "normalize factor " << normalize << std::endl;

		LOG_DEBUG << fileName << " matrix " << std::endl
			<< node->matrix[0][0] << " " << node->matrix[0][1] << " " << node->matrix[0][2] << " " << node->matrix[0][3] << std::endl
			<< node->matrix[1][0] << " " << node->matrix[1][1] << " " << node->matrix[1][2] << " " << node->matrix[1][3] << std::endl
			<< node->matrix[2][0] << " " << node->matrix[2][1] << " " << node->matrix[2][2] << " " << node->matrix[2][3] << std::endl
			<< node->matrix[3][0] << " " << node->matrix[3][1] << " " << node->matrix[3][2] << " " << node->matrix[3][3] << std::endl;

		glMultMatrixf(&node->matrix[0][0]);
		LOG_DEBUG << fileName << " pivot " << n->pivot[0] << " " <<n->pivot[1] << " " << n->pivot[2] << std::endl;
		glTranslatef(-n->pivot[0], -n->pivot[1], -n->pivot[2]);

		glScalef(normalize,normalize,normalize);
		glTranslatef(-bsphere.center.x,-bsphere.center.y,-bsphere.center.z);

		renderMeshImpl(mesh);

		glPopMatrix();

		glEndList();
	}

	glCallList(mesh->user_id);
}

void Model3ds::renderMeshImpl(Lib3dsMesh *mesh) {
	float (*normalL)[3] = (float(*)[3]) malloc(3 * 3 * sizeof(float) * mesh->nfaces);
	Lib3dsMaterial *oldmat = (Lib3dsMaterial *) -1;
	{
		float M[4][4];
		lib3ds_matrix_copy(M, mesh->matrix);
		lib3ds_matrix_inv(M);
		glMultMatrixf(&M[0][0]);
	}
	lib3ds_mesh_calculate_vertex_normals(mesh, normalL);

	oogl::TexturePtr texture;

	for (int p = 0; p < mesh->nfaces; ++p) {
		Lib3dsMaterial *mat = 0;

		if (mesh->faces[p].material >= 0) {
			mat = file->materials[mesh->faces[p].material];
		}

		if (mat != oldmat) { //material change
			if (mat) {
				if (mat->two_sided)
					glDisable(GL_CULL_FACE);
				else
					glEnable(GL_CULL_FACE);
				glDisable( GL_CULL_FACE);

				if (mat->texture1_map.name[0]) { /* texture map? */
					Lib3dsTextureMap *tex = &mat->texture1_map;
					LOG_INFO << "found texture " << tex->name << std::endl;
					if (tex->user_ptr == 0) { /*no texture yet*/
						std::string dir = "";
						if(fileName.find_last_of("/") != std::string::npos)
							dir = fileName.substr(0,fileName.find_last_of("/")+1);
						else if(fileName.find_last_of("\\") != std::string::npos)
							dir = fileName.substr(0,fileName.find_last_of("\\")+1);
						texture = oogl::loadTexture(dir + std::string(tex->name));
						textures.push_back(texture);
						tex->user_ptr = (void*)(int)textures.size();
					} else {
						LOG_DEBUG << "reuse texture " << (int)tex->user_ptr-1 << std::endl;
						texture = textures.at(((int)tex->user_ptr-1));
					}
				}

				{//apply material
					LOG_DEBUG << "custom material" << std::endl;
					float a[4], d[4], s[4];
					int i;
					for (i = 0; i < 3; ++i) {
						a[i] = mat->ambient[i];
						d[i] = mat->diffuse[i];
						s[i] = mat->specular[i];
					}
					a[3] = d[3] = s[3] = 1.0f;

					glMaterialfv(GL_FRONT, GL_AMBIENT, a);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
					glMaterialfv(GL_FRONT, GL_SPECULAR, s);
				}
				float shininess = pow(2.f, 10.0f * mat->shininess);
				glMaterialf(GL_FRONT, GL_SHININESS, shininess <= 128 ? shininess : 128);
			} else {
				LOG_DEBUG << "default material" << std::endl;
				const float a[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
				const float d[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
				const float s[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glMaterialfv(GL_FRONT, GL_AMBIENT, a);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
				glMaterialfv(GL_FRONT, GL_SPECULAR, s);
				glMaterialf(GL_FRONT, GL_SHININESS, pow(2.f, 10.0f * 0.5f));
			}
			oldmat = mat;
		} else if (mat != NULL && mat->texture1_map.name[0]) {
			Lib3dsTextureMap *tex = &mat->texture1_map;
			if (tex != NULL && tex->user_ptr != NULL) {
				texture = textures.at(((int)tex->user_ptr-1));
			}
		}

		{
			int i;

			GLboolean tex_was_enabled = glIsEnabled(GL_TEXTURE_2D);
			if (texture.get() != NULL) {
				LOG_DEBUG << "bind texture " << texture->getName() << std::endl;
				texture->bind(0);
			}

			glBegin( GL_TRIANGLES);
			{
				if (texture.get() != NULL) {
					for (i = 0; i < 3; ++i) {
						glNormal3fv(normalL[3 * p + i]);
						glTexCoord2f(mesh->texcos[mesh->faces[p].index[i]][0], mesh->texcos[mesh->faces[p].index[i]][1]);
						glVertex3fv(mesh->vertices[mesh->faces[p].index[i]]);
					}
				} else  {
					for (i = 0; i < 3; ++i) {
						glNormal3fv(normalL[3 * p + i]);
						glVertex3fv(mesh->vertices[mesh->faces[p].index[i]]);
					}
				}
			}
			glEnd();

			if (texture.get() != NULL) {
				texture->unbind();
				LOG_DEBUG << "unbind texture " << texture->getName() << std::endl;
				if(!tex_was_enabled) //restore original state
					glDisable(GL_TEXTURE_2D);
			}
		}
	}

	free(normalL);
}

void Model3ds::dump() {
	BoundingSphere bs = getBoundingSphere();
	LOG_WARN << fileName << std::endl
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
