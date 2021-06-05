#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 f_position;
in vec3 v_normal;

uniform vec3 u_lightColor;
uniform vec3 u_ambientLight;

uniform sampler2D u_Texture;
uniform vec3 u_LightPos;

void main() {

	// lightning
	vec3 normalized = normalize(v_normal);
	vec3 lightDir = normalize(u_LightPos - f_position);

	float diff = max(dot(normalized, lightDir), 0.0);
	vec3 diffuse = diff * u_lightColor;

	vec4 texColor = vec4(1.0); //texture(u_Texture, v_TexCoord);
	color = texColor * vec4(diffuse + u_ambientLight, 1.0);
};
