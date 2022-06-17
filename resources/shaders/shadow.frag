#version 460
in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	if (texture(tex, texCoords).a != 1)
		discard;
	else
		fragColor = vec4(0.0f, 0.0f, 0.0f, 0.35f);
}