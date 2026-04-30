#version 430 core

in vec3 relPos;
flat in vec3 worldCentre;
flat in float radius;

out vec4 fragCol;

uniform mat4 view;
uniform mat4 proj;
uniform vec3 camPos;
uniform vec3 bodyCol; // Diffuse

void main()
{
    // Calculate ray from camPos through current fragment
    vec3 localCamPos = (camPos - worldCentre) / radius; // Untranslate and unscale
    vec3 rayDir = normalize(relPos - localCamPos);

    // Solve quadratic for sphere intersection
    float B = 2 * dot(localCamPos, rayDir);
    float C = dot(localCamPos, localCamPos) - 1;

    float discr = B * B - 4 * C;
    if (discr < 0) discard;
    else
    {
        float t1 = (-B - sqrt(discr)) / 2.0; 
        float t2 = (-B + sqrt(discr)) / 2.0;
        float t = min(t1, t2);
        if (t <= 0) discard; // It is behind the camera

        vec3 localHit = localCamPos + t*rayDir;
        vec4 clipHit = proj * view * vec4((localHit * radius) + worldCentre, 1.0);
        gl_FragDepth =  clipHit.z / clipHit.w; // Manually do perspective division

        // LIGHTING
        // Diffuse
        vec3 pointLight = vec3(50.0, 10.0, 0.0); // hardcode for now
        vec3 L = normalize(pointLight - localHit);
        vec3 N = normalize(localHit);
        vec3 diffuse = bodyCol * max(0.0, dot(L, N));

        // Specular
        vec3 V = normalize(localCamPos - localHit);
        vec3 R = normalize((-L) - 2*(dot(-L, N)) * N);
        vec3 specCol = vec3(1.0, 1.0, 1.0);
        vec3 specular = specCol * max(0.0, pow(dot(R, V), 12));

        // Tone map
        vec3 finalCol = specular + diffuse;
        vec3 toneMapped = finalCol / (1.0 + finalCol);

        fragCol = vec4(toneMapped, 1.0);
    }
}