#version 330 core
out vec4 FragColor;

// uniform vec3 attrColor;

in vec3 ourColor; 
in vec2 texCoord; 

uniform vec4 uColor;

void main()
{
    FragColor = uColor;
}