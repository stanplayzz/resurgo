#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main() {
	FragPos = vec3(u_Model * vec4(aPosition, 1.0));
	Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	TexCoords = aTexCoords;

	gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
}