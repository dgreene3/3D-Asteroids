#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec2 UV;
out vec3 Normal;


void main() {
	gl_Position = projMatrix * viewMatrix * worldMatrix * vec4(VertexPosition, 1.0f);

	UV = VertexTexCoord;

	Normal = VertexNormal;
}