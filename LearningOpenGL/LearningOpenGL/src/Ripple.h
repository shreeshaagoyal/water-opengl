#pragma once

#include "Shader.h"

struct Ripple
{
	struct {
		float xpos;
		float ypos;
		float radius;
	} vertex;
		float radiusInc;
		float maxRadius;
		float thickness;
	//} data;
	bool deletePending = false;

	Ripple(float xpos_, float ypos_, float radius_, float radiusInc_, float maxRadius_);
	void UpdateRadius();
	std::array<float, 5 * 4> GetAsArray();
};
