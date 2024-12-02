#version 330 core

in vec3 fragPos;
in vec3 fragColor;
in vec3 n;

//uniform vec3 lightPosition;

in vec2 tc;
uniform sampler2D sampler;

out vec4 color;

void main() {
	// Sample texture
	vec4 d = texture(sampler, tc);

	if(d.a < 0.01)
		discard; // If the texture is transparent, don't draw the fragment

	color = d;
	// color = d * vec4(fragColor, 1.0);
}
