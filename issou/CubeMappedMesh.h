#pragma once
#include "TexturedMesh.h"

class CubeMappedMesh :
	public Mesh
{
public:
	CubeMappedMesh(const Transform& trans, const Shader& sh, const char *filename, const Visual& vis = Visual());
	~CubeMappedMesh();

private:
	void perform_draw() override;
};

