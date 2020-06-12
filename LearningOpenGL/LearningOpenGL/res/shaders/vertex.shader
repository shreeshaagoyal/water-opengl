#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 center;
layout(location = 2) in float radius;

out vec4 v_Center;
out float v_Radius;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_Center = center;
	v_Radius = radius;
};