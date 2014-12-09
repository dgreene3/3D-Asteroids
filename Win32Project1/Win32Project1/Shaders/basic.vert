#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;


out vec3 PosW;
out vec3 Normal;
out vec2 UV;




void main() {
	gl_Position = projMatrix * viewMatrix * worldMatrix * vec4(VertexPosition, 1.0f);

	PosW = vec3( worldMatrix * vec4(VertexPosition, 1.0f) );

	UV = VertexTexCoord;

	// Multiply normal with normal matrix

	mat3 normalMatrix = transpose(inverse( mat3(worldMatrix) ));
	Normal = normalize( normalMatrix * VertexNormal );
}