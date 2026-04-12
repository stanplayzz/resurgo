#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Color;

out vec4 FragColor;

uniform vec4 u_Color;
uniform vec3 u_LightDir;

uniform float u_PixelSize;
uniform float u_Scale;

float hash(vec2 p) { return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453); }

float valueNoise(vec2 p) {
	vec2 i = floor(p);
	vec2 f = fract(p);
	f = f * f * (3.0 - 2.0 * f);

	return mix(mix(hash(i), hash(i + vec2(1, 0)), f.x), mix(hash(i + vec2(0, 1)), hash(i + vec2(1, 1)), f.x), f.y);
}

float triplanarNoise(vec3 pos, float scale) {
	vec2 uvYZ = pos.yz * scale;
	uvYZ.x += valueNoise(pos.yz * scale + vec2(1.7, 9.2)) * 0.4;
	uvYZ.y += valueNoise(pos.yz * scale + vec2(8.3, 2.8)) * 0.4;

	vec2 uvXZ = pos.xz * scale;
	uvXZ.x += valueNoise(pos.xz * scale + vec2(3.1, 5.6)) * 0.4;
	uvXZ.y += valueNoise(pos.xz * scale + vec2(6.4, 1.9)) * 0.4;

	vec2 uvXY = pos.xy * scale;
	uvXY.x += valueNoise(pos.xy * scale + vec2(4.4, 7.3)) * 0.4;
	uvXY.y += valueNoise(pos.xy * scale + vec2(2.1, 6.8)) * 0.4;

	float n1 = valueNoise(uvYZ);
	float n2 = valueNoise(uvXZ);
	float n3 = valueNoise(uvXY);

	float n1b = valueNoise(uvYZ * 3.0 + vec2(4.1, 7.3));
	float n2b = valueNoise(uvXZ * 3.0 + vec2(5.2, 2.1));
	float n3b = valueNoise(uvXY * 3.0 + vec2(1.3, 8.4));

	float nx = n1 * 0.65 + n1b * 0.35;
	float ny = n2 * 0.65 + n2b * 0.35;
	float nz = n3 * 0.65 + n3b * 0.35;

	vec3 blend = abs(normalize(Normal));
	blend = pow(blend, vec3(4.0));
	blend /= (blend.x + blend.y + blend.z);

	return nx * blend.x + ny * blend.y + nz * blend.z;
}

void main() {
	vec3 lightColor = vec3(1.0);
	vec3 lightDir = normalize(u_LightDir);

	float ambientStrength = 0.7;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 lighting = ambient + diffuse;

	vec3 snappedPos = floor(FragPos / u_PixelSize) * u_PixelSize;
	float noise = triplanarNoise(snappedPos, u_Scale);

	noise = floor(noise * 3.0) / 3.0;

	float eps = 0.01;
	float nx = triplanarNoise(snappedPos + vec3(eps, 0.0, 0.0), u_Scale);
	float ny = triplanarNoise(snappedPos + vec3(0.0, eps, 0.0), u_Scale);
	float nz = triplanarNoise(snappedPos + vec3(0.0, 0.0, eps), u_Scale);
	vec3 noiseNormal = normalize(vec3((nx - noise) * 0.5, (ny - noise) * 0.5, (nz - noise) * 0.5 + 1.0));
	float bumpLight = clamp(dot(noiseNormal, lightDir), 0.0, 1.0);

	vec3 finalLighting = lighting * (0.85 + bumpLight * 0.25);

	vec3 finalColor = u_Color.rgb * finalLighting * Color;

	finalColor *= 1.0 + (noise - 0.5) * 0.2;

	FragColor = vec4(finalColor, u_Color.a);
}