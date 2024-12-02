#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 fragPos;
out vec3 fragColor;
out vec3 n;
out vec2 tc;

void main() {
	fragPos = pos;
	fragColor = color;
	n = normal;
	tc = texCoord;
	gl_Position = P * V * M * vec4(pos, 1.0);
}
