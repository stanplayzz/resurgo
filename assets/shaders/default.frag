#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;

void main() {
	vec3 lightColor = vec3(1, 1, 1);

	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(vec3(0.8, -0.6, 1));

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec4 texColor = texture(u_Texture, TexCoords);

	vec3 lighting = ambient + diffuse;

	if (u_UseTexture) {
		vec3 color = texColor.rgb * u_Color.rgb * lighting;
		float alpha = texColor.a * u_Color.a;
		FragColor = vec4(color, alpha);
	} else {
		vec3 color = u_Color.rgb * lighting;
		FragColor = vec4(color, u_Color.a);
	}
}