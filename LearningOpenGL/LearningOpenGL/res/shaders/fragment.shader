#version 330 core

out vec4 color;

in vec4 v_Center;
in vec4 gl_FragCoord;

uniform vec4 u_Color;
uniform float u_Radius;
uniform float u_Thickness;

void main()
{
	float hyp = length(gl_FragCoord - v_Center);
	if (hyp < 30.0f)
	{
		color = u_Color;
	}
	else
	{
		color = vec4(0.0, 0.0, 0.0, 0.0);
	}
};