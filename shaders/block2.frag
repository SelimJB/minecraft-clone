#version 330 core
out vec4 FragColor;

in vec2 TexCoord;


uniform sampler2D texture;


void main()
{
	vec2 toto = TexCoord;
	toto.x = (fract(toto.x)+15)/16;
	FragColor=texture(texture,vec2(0,0));
}