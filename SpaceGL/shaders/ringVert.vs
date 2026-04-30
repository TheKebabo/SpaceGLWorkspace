#version 430 core

layout (location = 0) in vec2 aVertex; // Quad corner
layout (location = 1) in mat4 instanceMatrix; // Transform quad to correct orientation

uniform mat4 view;
uniform mat4 proj;

out vec2 relPos; // For frag discarding
out vec3 worldPos;
out float worldRadius;

void main()
{
    relPos = aVertex;
    // Radius is the length of the first column of the model matrix
    worldRadius = max(0.01, length(vec3(instanceMatrix[0][0], instanceMatrix[0][1], instanceMatrix[0][2])));
    worldPos = (view * instanceMatrix * vec4(aVertex, 0.0f, 1.0)).xyz;
    gl_Position = proj * view * instanceMatrix * vec4(aVertex, 0.0f, 1.0);
}