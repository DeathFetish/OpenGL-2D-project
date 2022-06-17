#version 460
in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D tex;
uniform float time;

void main() 
{
	float amount = 0.5;
	vec4 color = vec4(0.2f, 0.2f, 0.2f, 0.1f);
	
	highp int x = int(gl_FragCoord.x / 4);
	highp int y = int(gl_FragCoord.y / 4);

	float randomIntensity = fract(1000 * sin(( x + y * time) * 3.14));
	amount *= randomIntensity;
	color.rgb += amount;
	
	fragColor = color;
}