#version 430 core
layout (location = 0) in vec3 aVertex; // Cube corner
layout (location = 1) in vec4 aPosRad;
flat out vec3 worldCentre;
flat out float radius;

uniform mat4 view;
uniform mat4 proj;

out vec3 relPos; // For frag discarding

void main()
{
    relPos = aVertex;

    worldCentre = aPosRad.xyz;
    radius = aPosRad.w;

    vec3 worldPos = (aVertex * radius) + worldCentre;

    gl_Position = proj * view * vec4(worldPos, 1.0);
}