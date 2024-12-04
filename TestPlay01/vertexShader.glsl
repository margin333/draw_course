#version 330 core

layout (location = 0) in vec3 aPos;

uniform vec3    iResolution;
uniform float   iTime;

out vec3    outResolution;
out float   outTime;
out vec2    fragCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    fragCoord = (aPos.xy + vec2(1.0, 1.0)) * 0.5 * iResolution.xy;

    outResolution=iResolution;
    outTime=iTime;
};