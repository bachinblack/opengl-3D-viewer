#ifndef VBOTEAPOT_H
#define VBOTEAPOT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "LightItem.h"

using glm::vec4;
using glm::vec3;
using glm::mat3;
using glm::mat4;

// new VBOTeapot(transform, shader, 16, glm::mat4(1), visual));
class VBOTeapot : public LightItem
{
public:
	VBOTeapot(const Transform& tr, const Shader& pr, const int grid, const mat4& lidTransform, const Visual& vis = Visual());

private:

	const int grid;

	void setup() override;
	void perform_draw() override;

    unsigned int faces;

    void generatePatches(float * v, float * n, float *tc, unsigned int* el, int grid);
    void buildPatchReflect(int patchNum,
                           float *B, float *dB,
                           float *v, float *n, float *, unsigned int *el,
                           int &index, int &elIndex, int &, int grid,
                           bool reflectX, bool reflectY);
    void buildPatch(vec3 patch[][4],
                    float *B, float *dB,
                    float *v, float *n,float *, unsigned int *el,
                    int &index, int &elIndex, int &, int grid, mat3 reflect, bool invertNormal);
    void getPatch( int patchNum, vec3 patch[][4], bool reverseV );

    void computeBasisFunctions( float * B, float * dB, int grid );
    vec3 evaluate( int gridU, int gridV, float *B, vec3 patch[][4] );
    vec3 evaluateNormal( int gridU, int gridV, float *B, float *dB, vec3 patch[][4] );
    void moveLid(int,float *,mat4);

};

#endif // VBOTEAPOT_H
