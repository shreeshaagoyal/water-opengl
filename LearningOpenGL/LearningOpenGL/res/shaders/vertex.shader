#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 center;

out vec4 v_Center;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_Center = center;
};