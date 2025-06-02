#version 330 core
out vec4 FragColor;

// Capture variables from previous stages
in vec3 ourColor;
in vec2 texCoord;

// Texture defined by uniform
uniform sampler2D ourTexture;

void main()
{
    // Set color of the fragment by mapping the
    // texture into the mesh using UVs
    FragColor = texture(ourTexture, texCoord);
}

