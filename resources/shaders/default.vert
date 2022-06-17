#version 460
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 textureCoords;

out vec2 texCoords;

uniform mat4 viewProjectionMat;
uniform mat4 modelMat;
uniform float layer;

void main()
{
	texCoords = textureCoords;
	gl_Position = viewProjectionMat * modelMat * vec4(vertexPosition, layer, 1.0);
}