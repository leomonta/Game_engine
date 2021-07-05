#version 460 core

layout(location = 0) out vec4 color;

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in vec3 f_position;
layout(location = 2) in vec3 v_normal;

uniform vec3 u_LightColor;

uniform sampler2D u_Texture;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

void main() {

	// lightning

	// ambient
	vec3 ambient = u_LightColor * 0.2;

	// diffuse
	vec3 normalized = normalize(v_normal);
	vec3 lightDir = normalize(u_LightPos - f_position);
	float diffLight = max(dot(normalized, lightDir), 0.0);
	vec3 diffuse = diffLight * u_LightColor;

	// specular
	vec3 viewDir = normalize(u_CameraPos - f_position);
	vec3 reflectDir = reflect(-lightDir, normalized);
	float specLight = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = 0.5 * specLight * u_LightColor;

	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor * vec4(ambient + diffuse + specular, 1.0);
};
