#version 400 core

in vec3 vertexColor;

out vec4 OutColor;

//uniform sampler2D texture_diffuse1;

void main() {
	OutColor = vec4(vertexColor, 1.0);
}
