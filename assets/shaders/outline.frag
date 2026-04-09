#version 330 core

in vec2 TexCoords;

uniform float u_OutlineThickness;
uniform vec4 u_OutlineColor;

out vec4 FragColor;

void main() {
	bool onEdge = TexCoords.x < u_OutlineThickness || TexCoords.x > 1.0 - u_OutlineThickness ||
				  TexCoords.y < u_OutlineThickness || TexCoords.y > 1.0 - u_OutlineThickness;

	if (!onEdge) discard;

	FragColor = u_OutlineColor;
}