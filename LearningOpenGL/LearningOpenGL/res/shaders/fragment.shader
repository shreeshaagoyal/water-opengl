#version 330 core

out vec4 color;

in vec4 gl_FragCoord;
in vec4 v_Center;
in float v_Radius;

uniform vec4 u_Color;

void main()
{
	float hyp = length(gl_FragCoord - v_Center);
	float thickness = 40.0f;
	float maxRadius = 400.0f;
	float hypWithMaxAlpha = 100.0f;
	//float hypWithMaxAlpha = maxRadius / 2.0f;
	if (((v_Radius - thickness) < hyp) && (hyp < v_Radius))
	{
		color = u_Color;
		float baseAlpha = 0.0f;
		if (hyp < hypWithMaxAlpha)
		{
			baseAlpha = (1 - ((hypWithMaxAlpha - hyp) / hypWithMaxAlpha)) * 0.5f;
		}
		else
		{
			baseAlpha = (1 - ((hyp - hypWithMaxAlpha) / (maxRadius - hypWithMaxAlpha))) * 0.5f;
		}

		float thicknessWithMaxAlpha = thickness / 2;
		if ((v_Radius - hyp) < thicknessWithMaxAlpha)
		{
			color[3] = ((v_Radius - hyp) / thicknessWithMaxAlpha) * baseAlpha;
		}
		else
		{
			color[3] = (1 - ((v_Radius - hyp - thicknessWithMaxAlpha) / thicknessWithMaxAlpha)) * baseAlpha;
		}
	}
	else
	{
		color = vec4(0.0, 0.0, 0.0, 0.0);
	}
};