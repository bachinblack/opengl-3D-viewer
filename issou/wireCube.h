#pragma once
#include "AItem.h"
class wireCube :
	public AItem
{
public:
	wireCube(const Transform& trans, const Shader &pr);
	~wireCube();

private:
	
	void setup() override;
	void perform_draw() override;

};

