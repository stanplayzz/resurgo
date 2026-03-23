#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;

void main() {
	vec4 texColor = texture(u_Texture, TexCoords);

	if (u_UseTexture) {
		FragColor = texColor * u_Color;
	} else {
		FragColor = u_Color;
	}
}