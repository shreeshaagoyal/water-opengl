#version 330 core

out vec4 color;

in vec2 v_TexCoord;
in vec4 gl_FragCoord;

uniform vec4 u_Color;
uniform vec4 u_Center;
uniform float u_Radius;

void main()
{
	if (length(gl_FragCoord - u_Center) < u_Radius)
	{
		color = u_Color;
	}
	else
	{
		color = vec4(0.0, 0.0, 0.0, 0.0);;
	}
};