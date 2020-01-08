#shader vertex
#version 410 core

layout(location = 0) in vec3 vPos;

void main()
{
    gl_Position = vec4(vPos, 1.0);
}

#shader fragment
#version 410 core

uniform vec4 color;

out vec4 FragColor;
void main()
{
    FragColor = color;
}