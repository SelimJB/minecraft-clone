#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform float mixValue;
uniform vec3 ourColor2;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
	FragColor=mix(texture(texture1,TexCoord),texture(texture2,vec2(1-TexCoord.x,TexCoord.y)*2), 0.5) * vec4(ourColor2,1) + vec4(0.1,0.1,0.1,1);
}