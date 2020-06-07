#include "Ripple.h"

Ripple::Ripple(double xpos_, double ypos_, float radius_, float radiusInc_, float maxRadius_, float thickness_)
	: xpos(xpos_), ypos(ypos_), radius(radius_), radiusInc(radiusInc_), maxRadius(maxRadius_), thickness(thickness_)
{}

void Ripple::UpdateRadius(Shader& shader)
{
	shader.SetUniform1f("u_Radius", radius);
	if (radius > (maxRadius - thickness))
		deletePending = true;

	radius += radiusInc;
}
