#version 330 core
layout(location=0)in vec4 coord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 texcoord;

void main(void) {
	texcoord = coord;

	gl_Position = projection * view * model * vec4(coord.xyz, 1.0);
}
