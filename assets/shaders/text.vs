#version 330

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

layout(location = 0) in vec3 vsiPosition;
layout(location = 1) in vec3 vsiNormal;
layout(location = 2) in vec2 vsiTexCoord;

out vec2 vsoTexCoord;

void main(void) {
  vsoTexCoord = vec2(vsiTexCoord.s, 1 - vsiTexCoord.t);
  gl_Position = projectionMatrix * modelMatrix * vec4(vsiPosition, 1.0);
}
