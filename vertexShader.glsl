#version 330 core

layout (location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vColor;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   vColor = vec4(clamp(aPos, 0.0f, 1.0f), 1.0f);
};