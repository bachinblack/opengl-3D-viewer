#pragma once

#include "AItem.h"

class CheckeredFloor : public AItem
{
public:
	CheckeredFloor(const Transform& trans, const Shader &pr, const int s, const int ns);
	~CheckeredFloor();

private:

	const int size;
	const int nsquares;
	
	void setup() override;
	void perform_draw() override;
};

