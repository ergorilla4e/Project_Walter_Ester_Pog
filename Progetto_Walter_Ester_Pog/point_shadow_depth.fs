#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    vec3 fragPosNorm = FragPos.xyz/FragPos.w;

    float lightDistance = length(fragPosNorm - lightPos);
    
    lightDistance = lightDistance / far_plane;
    
    gl_FragDepth = lightDistance;
};