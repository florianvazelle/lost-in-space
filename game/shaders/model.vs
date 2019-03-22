#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout(location = 0) in vec3 vsiPosition;
layout(location = 1) in vec3 vsiNormal;
layout(location = 2) in vec2 vsiTexCoord;

out vec2 vsoTexCoord;
out vec3 vsoNormal;
out vec4 vsoModPosition;

void main(void) {
  vsoTexCoord = vsiTexCoord;
  vsoNormal = (transpose(inverse(modelMatrix)) * vec4(vsiNormal, 0.0)).xyz;
  vsoModPosition = modelMatrix * vec4(vsiPosition, 1.0);
  gl_Position =
      projectionMatrix * viewMatrix * modelMatrix * vec4(vsiPosition, 1.0);
}
