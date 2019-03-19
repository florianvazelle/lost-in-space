#version 330 core
layout (location = 0) in vec3 vsiPosition;
out vec3 TexCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
    vec4 pos = projectionMatrix * viewMatrix * vec4(vsiPosition, 1.0);
    gl_Position = pos.xyww;
    TexCoords = vsiPosition;
}
