#include "Ripple.h"

#include <array>

Ripple::Ripple(float xpos_, float ypos_, float radius_, float radiusInc_, float maxRadius_)
{
	vertex.xpos = xpos_;
	vertex.ypos = ypos_;
	vertex.radius = radius_;
	radiusInc = radiusInc_;
	maxRadius = maxRadius_;
}

void Ripple::UpdateRadius()
{
	vertex.radius += radiusInc;
	if (vertex.radius > maxRadius)
		deletePending = true;
}

std::array<float, 5 * 4> Ripple::GetAsArray()
{
	float x = vertex.xpos;
	float y = vertex.ypos;
	float radius = vertex.radius;
	return {
		x - radius, y - radius, x, y, radius,
		x + radius, y - radius, x, y, radius,
		x + radius, y + radius, x, y, radius,
		x - radius, y + radius, x, y, radius
	};
}
