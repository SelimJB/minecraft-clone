#version 330 core
layout(location=0)in vec4 aPos;
attribute vec4 coord;
uniform mat4 mvp;
varying vec4 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vColor;

void main(void) {
	// Just pass the original vertex coordinates to the fragment shader as texture coordinates
	texcoord = coord;
	// Grass
	if (aPos.w == 4){
		vColor = vec4(vec3(58,179,45)/255,1);
	}
	// Dirt
	else if (aPos.w == 2){
		vColor = vec4(vec3(150,80,80)/255,1);
	}
	// Water
	else if (aPos.w == 8){
		vColor = vec4(vec3(30,200,245)/255,1);
	}	
	// Sand
	else if (aPos.w == 4){
		vColor = vec4(vec3(50,50,50)/255,1);
	}

	// Apply the model-view-projection matrix to the xyz components of the vertex coordinates
	gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
}
