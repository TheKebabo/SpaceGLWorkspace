#version 430 core
layout (location = 0) in vec3 aVertex; // Cube corner
// Instance data
layout (location = 1) in vec4 aPosRad;
layout (location = 2) in vec2 aTexIndices; // Diffuse, specular

flat out vec3 worldCentre;
flat out float worldRadius;
flat out vec2 texIndices;
out vec3 relPos; // For frag discarding

uniform mat4 view;
uniform mat4 proj;


void main()
{
    relPos = aVertex;

    worldCentre = aPosRad.xyz;
    worldRadius = aPosRad.w;
    texIndices = aTexIndices;

    vec3 worldPos = (aVertex * worldRadius) + worldCentre;

    gl_Position = proj * view * vec4(worldPos, 1.0);
}