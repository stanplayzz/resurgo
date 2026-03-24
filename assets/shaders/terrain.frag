#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Color;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform vec4 u_Color;
uniform vec3 u_LightDir;
uniform sampler2D u_Texture;
uniform sampler2D u_ShadowMap;
uniform bool u_UseTexture;

float getShadow(vec4 fragPosLightSpace) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float bias = max(0.05 * (1.0 - dot(normalize(Normal), normalize(u_LightDir))), 0.005);

	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	return shadow;
}

void main() {
	vec3 lightColor = vec3(1.0);

	vec3 lightDir = normalize(u_LightDir);

	float ambientStrength = 0.7;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float shadow = getShadow(FragPosLightSpace);

	vec3 lighting = ambient + (1.0 - shadow) * diffuse;
	vec3 finalColor;

	if (u_UseTexture) {
		vec4 texColor = texture(u_Texture, TexCoords);
		finalColor = texColor.rgb * lighting;
		finalColor *= Color;
	} else {
		finalColor = u_Color.rgb * lighting * Color;
	}

	FragColor = vec4(finalColor, u_Color.a);
}