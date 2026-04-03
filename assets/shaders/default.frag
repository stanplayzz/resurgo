#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Color;

out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;
uniform vec2 u_TexRectOffset;
uniform vec2 u_TexRectSize;

void main() {
	if (u_UseTexture) {
		vec2 uv = u_TexRectOffset + TexCoords * u_TexRectSize;
		vec4 texColor = texture(u_Texture, uv);
		FragColor = vec4(texColor.rgb * Color, texColor.a);
	} else {
		FragColor = vec4(u_Color.rgb * Color, u_Color.a);
	}
}