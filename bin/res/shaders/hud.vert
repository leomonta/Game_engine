#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec4 Tint;
layout(location = 4) in float Text_ID;

out vec2 v_TexCoord;
out vec4 v_color;

void main() {
	v_TexCoord = TexCoord;
	gl_Position = position;
	v_color = Tint;
};