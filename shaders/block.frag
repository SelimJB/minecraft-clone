#version 330 core
out vec4 FragColor;

uniform vec3 myColor;

in vec4 vColor;
void main()
{
	// FragColor = vec4(myColor,1);
	FragColor = vColor;
}