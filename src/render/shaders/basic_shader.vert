#version 330 core

// Define the Layouts taken from the CPU defined VAO
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

// Define variables passed to following stages
out vec3 ourColor;
out vec2 texCoord;

void main()
{
    // Set position
    gl_Position = vec4(aPos, 1.0);

    // Pass variables to the following stages
    ourColor = aColor;
    texCoord = aTexCoord;
}

