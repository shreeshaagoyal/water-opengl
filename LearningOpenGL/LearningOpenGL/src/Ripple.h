#pragma once

#include "Shader.h"

struct Ripple
{
	double xpos;
	double ypos;
	float radius;
	float radiusInc;
	float maxRadius;
	float thickness;
	bool deletePending = false;
	Ripple(double xpos_, double ypos_, float radius_, float radiusInc_, float maxRadius_, float thickness_);
	void UpdateRadius(Shader& shader);
};
