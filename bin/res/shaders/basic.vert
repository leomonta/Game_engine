#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec4 Tint;
layout(location = 4) in float Text_ID;

layout(location = 0) out vec2 v_TexCoord;
layout(location = 1) out vec3 v_normal;
layout(location = 2) out vec3 f_position;

uniform mat4 u_MVP;
uniform mat4 u_Model;

void main() {
	v_TexCoord = TexCoord;
	v_normal = normal;
	gl_Position = u_MVP * position;
	f_position = vec3(u_Model * position);
};
