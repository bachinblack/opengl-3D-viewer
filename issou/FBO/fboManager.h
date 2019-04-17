/*
* File:   fboManager.h
* Desc:		Jednoduchy "manager" pro FBO objekty
*
*/


#define GLM_ENABLE_EXPERIMENTAL 


#ifndef FBOMANAGER_H
#define FBOMANAGER_H


#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

class FboManager {
public:
	FboManager();
	~FboManager();
	//check fbo status
	bool checkFboStatus();

	//create a renderbuffer object
	void genRenderDepthBuffer(unsigned w, unsigned h);

	//bind a renderbuffer object
	void bindRenderDepthBuffer();

	void bindToFbo(GLenum type, GLenum texture, GLuint textureId);
	
	void bind3DTextureToFbo(GLenum type, GLuint textureId);
	void initFbo();
	void setDrawBuffers();
	GLuint getFboId();
private:
	GLuint _fboId;
	GLuint _renderDepthBufferId;
	bool useRenderDepthBuffer;
	unsigned attachmentCount;
	GLenum mrt[6];
};

#endif