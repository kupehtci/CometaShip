#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 uModel;
uniform mat4 uViewProjection;

void main()
{
    FragPos = vec3(uModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(uModel))) * aNormal;
    gl_Position = uViewProjection * vec4(FragPos, 1.0);
}