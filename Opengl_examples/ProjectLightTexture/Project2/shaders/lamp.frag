#version 420 core
out vec4 color;

uniform vec3 lightColor;

void main()
{
    color = vec4(lightColor, 1.0f); // Set alle 4 vector values to 1.0f
}