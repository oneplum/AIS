#version 410 core

in vec3 sColor;
out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(sColor, 1.0f);
}
