#pragma once
#include "AItem.h"
class Light :
	public AItem
{
public:
	Light(const Transform& trans, const Shader &pr);
	~Light() {};

private:

	void setup() override;
	void perform_draw() override;
};