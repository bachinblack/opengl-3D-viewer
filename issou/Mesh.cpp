
#include <vector>
#include <iostream>
#include "Mesh.h"

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/SOIL.h>

/**
*	Constructor, loading the specified aiMesh
**/

#include "stb/stb_image.h"

std::string root = "C:/Users/54604/Downloads/Sponza-master/";


Mesh::MeshEntry::MeshEntry(const unsigned short id, const aiScene* scene, Mesh * m)
{
	parent = m;

	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;

	aiMesh *mesh = scene->mMeshes[id];


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	elementCount = mesh->mNumFaces * 3;


	if (mesh->HasPositions()) {
		float *vertices = new float[mesh->mNumVertices * 3];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		delete[] vertices;
	}


	if (mesh->HasTextureCoords(0)) {
		float *texCoords = new float[mesh->mNumVertices * 2];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		delete[] texCoords;
	}


	if (mesh->HasNormals()) {
		float *normals = new float[mesh->mNumVertices * 3];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1, &vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		delete[] normals;
	}


	if (mesh->HasFaces()) {
		unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		delete[] indices;
	}

	// Tangent space for normal
	if (mesh->HasTangentsAndBitangents()) {

		float *tangent = new float[mesh->mNumVertices * 3];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			tangent[i * 3] = mesh->mTangents[i].x;
			tangent[i * 3 + 1] = mesh->mTangents[i].y;
			tangent[i * 3 + 2] = mesh->mTangents[i].z;
		}
		float *bittangent = new float[mesh->mNumVertices * 3];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			bittangent[i * 3] = mesh->mBitangents[i].x;
			bittangent[i * 3 + 1] = mesh->mBitangents[i].y;
			bittangent[i * 3 + 2] = mesh->mBitangents[i].z;
		}

		glGenBuffers(1, &vbo[TANGENT]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TANGENT]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), tangent, GL_STATIC_DRAW);

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);
	
		glGenBuffers(1, &vbo[BITTANGENT]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[BITTANGENT]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), bittangent, GL_STATIC_DRAW);

		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(5);
	}


	texID = mesh->mMaterialIndex;
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		material->Get(AI_MATKEY_SHININESS, shininessStrength);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, dcolor);
		material->Get(AI_MATKEY_COLOR_AMBIENT, acolor);
		material->Get(AI_MATKEY_COLOR_SPECULAR, scolor);
		
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) { //we only care diffuse texture
			aiString Path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				texpath = Path.data;
			}
		}
		//if (material->GetTextureCount(aiTextureType_NORMALS) > 0) {
		//	aiString Path;
		//	if (material->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
		//		normpath = Path.data;
		//		std::cout << normpath << std::endl;
		//	}
		//}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/**
*	Deletes the allocated OpenGL buffers
**/
Mesh::MeshEntry::~MeshEntry() {
	if (vbo[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
	}

	if (vbo[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
	}

	if (vbo[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
	}

	if (vbo[INDEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
	}

	if (vbo[TANGENT]) {
		glDeleteBuffers(1, &vbo[TANGENT]);
	}

	if (vbo[BITTANGENT]) {
		glDeleteBuffers(1, &vbo[BITTANGENT]);
	}
	glDeleteVertexArrays(1, &vao);

	
}

/**
*	Renders this MeshEntry
**/
void Mesh::MeshEntry::render() {
	
	glBindVertexArray(vao);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/ sizeof(unsigned int), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

/**
*	Mesh constructor, loads the specified filename if supported by Assimp
**/
Mesh::Mesh(const Transform& trans, const Shader& sh, const char *filename, const Visual& vis)
	: LightItem(trans, sh, vis, true)
{
	shader = sh;

	setRoot(filename);
	
	Assimp::Importer importer;   //aiProcessPreset_TargetRealtime_Fast
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		meshEntries.push_back(new Mesh::MeshEntry(i, scene, this));
	}
}


void Mesh::setRoot(const std::string& name)
{
	name.substr();
	std::size_t found = name.find_last_of("/\\");
	_root = name.substr(0, found) + '/';
}

/**
*	Clears all loaded MeshEntries
**/
Mesh::~Mesh(void)
{
	for (unsigned int i = 0; i < meshEntries.size(); ++i) {

		delete meshEntries.at(i);
	}
	meshEntries.clear();
}

/**
*	Renders all loaded MeshEntries
**/
void Mesh::perform_draw() {

	for (unsigned int i = 0; i < meshEntries.size(); ++i) {

		MeshEntry * m = meshEntries[i];
		
		glUniform1i(this->shader.program->uniform("text"), 0);
		glUniform1f(shader.program->uniform("Shininess"), (m->shininessStrength || 10.0f));

		m->render();
	}
}
