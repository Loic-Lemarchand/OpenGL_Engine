#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord; 


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vColor;
out vec2 vTexCoord;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   //vColor = vec4(clamp(aPos, 0.0f, 1.0f), 1.0f);
   vTexCoord = aTexCoord;
};