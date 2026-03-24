#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Color;
out vec4 FragPosLightSpace;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform mat4 u_LightSpaceMatrix;

void main() {
	FragPos = vec3(u_Model * vec4(aPosition, 1.0));
	Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	TexCoords = aTexCoords;
	FragPosLightSpace = u_LightSpaceMatrix * vec4(FragPos, 1.0);
	Color = aColor;

	gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
}