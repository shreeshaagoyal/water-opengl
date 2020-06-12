#version 330 core

out vec4 color;

in vec4 gl_FragCoord;
in vec4 v_Center;
in float v_Radius;

uniform vec4 u_Color;

void main()
{
	float hyp = length(gl_FragCoord - v_Center);
	if (((v_Radius - 2.0f) < hyp) && (hyp < v_Radius))
	{
		color = u_Color;
	}
	else
	{
		color = vec4(0.0, 0.0, 0.0, 0.0);
	}
};