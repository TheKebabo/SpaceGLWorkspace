#version 430 core

in vec2 relPos; // In local space
in vec3 worldPos; // In world space
in float worldRadius;
out vec4 fragCol;

uniform mat4 proj;
uniform vec3 ringCol;

void main()
{
    float localDistCentre = length(relPos);
    float deltaDist = fwidth(localDistCentre); // The max variation of the local distance value in relation to surrounding fragments

    float thickness = 0.2;

    float innerEdge = 1.0 - ((thickness/2.0) / worldRadius);
    float innerAlpha = smoothstep(innerEdge - deltaDist, innerEdge, localDistCentre); // Scale from 0 to 1 as dist increases

    float outerEdge = 1.0;
    float outerAlpha = 1.0 - smoothstep(innerEdge - deltaDist, outerEdge, localDistCentre); // Scale from 1 to 0 as dist increases
    
    float alpha = innerAlpha * outerAlpha;
    if (alpha < 0.01) discard;

    fragCol = vec4(ringCol, alpha);

    vec4 clip = proj * vec4(worldPos, 1.0);
    gl_FragDepth = clip.z / clip.w;
}