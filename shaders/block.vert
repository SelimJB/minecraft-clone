#version 330 core
layout(location=0)in vec4 aPos;
attribute vec4 coord;
uniform mat4 mvp;
varying vec4 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vColor;

vec4 GetColor(float blockType){
	vec3 color = vec3(0);
	// Dirt
	if (blockType == 1)
		color = vec3(150,80,80);
	// TopSoil
	else if (blockType == 2)
		color = vec3(0,153,15);
	// Grass
	else if (blockType == 3)
		color = vec3(58,179,45);
	// Leaves 
	else if (blockType == 4)
		color = vec3(0,120,8);
	// 	Wood
	else if (blockType == 5)
		color = vec3(122,81,52);
	// Stone
	else if (blockType == 6)
		color = vec3(130);
	// Sand
	else if (blockType == 7)
		color = vec3(255,217,96);
	// Water
	else if (blockType == 8)
		color = vec3(30,200,245);
	// Brick
	else if (blockType == 10)
		color = vec3(255,217,96);
	// Ore
	else if (blockType == 11)
		color = vec3(255,217,96);		
	// Woodrings
	else if (blockType == 12)
		color = vec3(255,217,96);				
	else 
		return vec4(0);
	return vec4(color / 255, 1);
}

void main(void) {
	// Just pass the original vertex coordinates to the fragment shader as texture coordinates
	texcoord = coord;
	vColor = GetColor(aPos.w);
	// Apply the model-view-projection matrix to the xyz components of the vertex coordinates
	gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
}
