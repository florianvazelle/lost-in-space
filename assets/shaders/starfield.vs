#version 330

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

layout(location = 0) in vec3 vsiPosition;
layout(location = 1) in vec3 vsiNormal;
layout(location = 2) in vec2 vsiTexCoord;

out vec3 vsoNormal;
out vec3 vsoModPos;
out vec2 vsoTexCoord;

void main(void) {
  vsoTexCoord = vsiTexCoord;
  vec4 mp = modelMatrix * vec4(vsiPosition, 1.0);
  vsoNormal = (transpose(inverse(modelMatrix)) * vec4(vsiNormal, 0.0)).xyz;
  vsoModPos = mp.xyz;
  gl_Position = projectionMatrix * mp;
}
