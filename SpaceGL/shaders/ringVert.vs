#version 430 core

layout (location = 0) in vec2 aVertex; // Quad corner
layout (location = 1) in mat4 instanceMatrix; // Transform quad to correct orientation

uniform mat4 view;
uniform mat4 proj;

out vec2 relPos; // For frag discarding
out vec3 viewPos;
out vec2 worldScale;

void main()
{
    relPos = aVertex;

    worldScale.x = max(0.01, length(instanceMatrix[0]));
    worldScale.y = max(0.01, length(instanceMatrix[1]));

    viewPos = (view * instanceMatrix * vec4(aVertex, 0.0f, 1.0)).xyz;
    gl_Position = proj * view * instanceMatrix * vec4(aVertex, 0.0f, 1.0);
}