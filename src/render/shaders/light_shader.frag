#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;

    bool hasDiffuseMap;
    bool hasSpecularMap;
    bool hasEmissionMap;

    vec3 diffuse;
    vec3 specular;

    vec3 color;
    vec3 ambient;
    float shininess;
};

uniform Material material;

in vec3 ourColor; 

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0) * vec4(material.color, 1.0);
}