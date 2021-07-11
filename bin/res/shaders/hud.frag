
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_color;

uniform sampler2D u_Texture;

void main () {
	color = v_color * texture(u_Texture, v_TexCoord);
};