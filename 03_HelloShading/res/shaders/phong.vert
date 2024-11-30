#version 410 core

layout(location = 0) in vec3 vertexPosition;  // vertex position in object/model space
layout(location = 1) in vec3 vertexNormal;    // vertex normal in object/model space

uniform mat4 MVP; // model-view-projection Matrix
uniform mat4 MV;
uniform mat4 MVit;

out vec3 positionViewSpace;
out vec3 normalViewSpace;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1);

    positionViewSpace = vec3(MV * vec4(vertexPosition, 1));
    normalViewSpace = normalize((MVit * vec4(vertexNormal, 0)).xyz);
}
