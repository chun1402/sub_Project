#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aColor;

//out vec2 TexCoords;
out vec3 vertexColor;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

//uniform int pointSize; //point size 설정
uniform mat4 MVP;
uniform vec3 uniformColor; // 색상 설정

void main() {
	//TexCoords = aTexCoords;
	//gl_Position = projection * view * model * vec4(aPos, 1.0);
	gl_Position = MVP * vec4(aPos, 1.0);
	//gl_PointSize = pointSize;
	vertexColor = aColor;
}
