#pragma once

#include "Shader.h"

struct Ripple
{
	struct {
		float xpos;
		float ypos;
	} data;
		float radius;
		float radiusInc;
		float maxRadius;
		float thickness;
	//} data;
	bool deletePending = false;
	Ripple(float xpos_, float ypos_);
	Ripple(float xpos_, float ypos_, float radius_, float radiusInc_, float maxRadius_, float thickness_);
	void UpdateRadius();
	auto getAsArray();
	std::array<float, 5 * 4> getPositions();
};
