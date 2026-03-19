#version 330 core

uniform sampler2D texture1;

out vec4 FragColor;

in vec2 vTexCoord;

void main()
{

   FragColor = texture(texture1, vTexCoord);
};