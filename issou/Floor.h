#pragma once

#include "LightItem.h"

class Floor : public LightItem
{
public:
	Floor(const Transform& trans, const Shader &pr, const int s, const int ns);
	~Floor();
};

