#include "Ripple.h"

#include <array>

Ripple::Ripple(float xpos_, float ypos_)
{
	data.xpos = xpos_;
	data.ypos = ypos_;
	radius = 30.0f;
	radiusInc = 0.0f;
	maxRadius = 30.0f;
	thickness = 0.0f;
}

Ripple::Ripple(float xpos_, float ypos_, float radius_, float radiusInc_, float maxRadius_, float thickness_)
{
	data.xpos = xpos_;
	data.ypos = ypos_;
	radius = radius_;
	radiusInc = radiusInc_;
	maxRadius = maxRadius_;
	thickness = thickness_;
}

void Ripple::UpdateRadius()
{
	if (radius < maxRadius)
	{
		radius += radiusInc;
	}
}

std::array<float, 5 * 4> Ripple::getPositions()
{
	float x = data.xpos;
	float y = data.ypos;
	return {
		x - radius, y - radius, x, y, radius,
		x + radius, y - radius, x, y, radius,
		x + radius, y + radius, x, y, radius,
		x - radius, y + radius, x, y, radius
	};
}
