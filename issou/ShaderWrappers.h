#pragma once

#include <vector>

#include "AItem.h"
#include "Light.h"


//std::string SH_DIR = "./shaders/";

ShaderProgram *createShaderProgram(const std::string shname, const std::vector<std::string>& uniforms);


class AShaderWrapper
{
public:
	AShaderWrapper();
	~AShaderWrapper();

	void draw(const glm::mat4& view, const glm::mat4& projection);

	void computeLight(std::vector<Light *> lights);
	void addItem(AItem *it);
	void clearItems(void);
	void addItems(const std::vector<AItem *>& its);
	std::vector<AItem *> getItems(void) const;

	ShaderProgram *getShaderProgram(void) const;

protected:
	virtual void setUniforms(const glm::mat4& projection);
	virtual void perform_draw(const glm::mat4& view);

	ShaderProgram		*_sh;
	std::vector<AItem *> _items;
	
	glm::vec4			*_lightPos;
	glm::vec3			*_lightCol;
	unsigned short		 _lightNb;
};


class ShaderSimple : public AShaderWrapper {
public:
	ShaderSimple();
private:
};


class ShaderPhong : public AShaderWrapper {
public:
	ShaderPhong();
private:
	void setUniforms(const glm::mat4& projection) override;
};


class ShaderToon : public AShaderWrapper {
public:
	ShaderToon();
private:
	void setUniforms(const glm::mat4& projection) override;
};

class ShaderSilouette : public AShaderWrapper {
public:
	ShaderSilouette();
private:
	float _thickness;

	void setUniforms(const glm::mat4& projection) override;
	void perform_draw(const glm::mat4& view) override;
};