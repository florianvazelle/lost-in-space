#version 330

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform int type;
uniform float getTime;

layout(location = 0) in vec3 vsiPosition;
layout(location = 1) in vec3 vsiNormal;
layout(location = 2) in vec2 vsiTexCoord;

out vec3 vsoNormal;
out vec3 vsoModPos;
out vec2 vsoTexCoord;
out float vsoColor;

#define MOD3 vec3(.1031, .11369, .13787)

vec3 hash33(vec3 p3) {
  p3 = fract(p3 * MOD3);
  p3 += dot(p3, p3.yxz + 19.19);
  return -1.0 +
         2.0 * fract(vec3((p3.x + p3.y) * p3.z, (p3.x + p3.z) * p3.y,
                          (p3.y + p3.z) * p3.x));
}

float perlin_noise(vec3 p) {
  vec3 pi = floor(p);
  vec3 pf = p - pi;

  vec3 w = pf * pf * (3.0 - 2.0 * pf);

  return mix(mix(mix(dot(pf - vec3(0, 0, 0), hash33(pi + vec3(0, 0, 0))),
                     dot(pf - vec3(1, 0, 0), hash33(pi + vec3(1, 0, 0))), w.x),
                 mix(dot(pf - vec3(0, 0, 1), hash33(pi + vec3(0, 0, 1))),
                     dot(pf - vec3(1, 0, 1), hash33(pi + vec3(1, 0, 1))), w.x),
                 w.z),
             mix(mix(dot(pf - vec3(0, 1, 0), hash33(pi + vec3(0, 1, 0))),
                     dot(pf - vec3(1, 1, 0), hash33(pi + vec3(1, 1, 0))), w.x),
                 mix(dot(pf - vec3(0, 1, 1), hash33(pi + vec3(0, 1, 1))),
                     dot(pf - vec3(1, 1, 1), hash33(pi + vec3(1, 1, 1))), w.x),
                 w.z),
             w.y);
}

float generate_planet(float level, bool useTime) {
  const float PI = 3.1415926535897932384626433832795;
  float PI180 = float(PI / 180.0);

  float lat = vsiTexCoord.y * 180 - 90;
  float lon = vsiTexCoord.x * 360 - 90;
  float r = cos(PI180 * lat);

  float _x = (r * cos(PI180 * lon));
  float _y = (sin(PI180 * lat));
  float _z = (r * sin(PI180 * lon));

  vec3 TexCoord = level * vec3(_x, _y, _z);
  float color = perlin_noise(TexCoord);

  if (useTime)
    color = sin(color * 20. + getTime);
  else
    color = sin(color * 20.);

  return color;
}

float which_planet() {
  if (type == 1) {
    return generate_planet(8, true);
  } else if (type == 2) {
    return generate_planet(1, false);
  } else if (type == 3) {
    return generate_planet(2, false);
  }
}

void main(void) {
  vsoTexCoord = vsiTexCoord;
  vsoColor = which_planet();
  vec3 bpos =
      (type != 0) ? vsiPosition + 0.04 * vsoColor * vsiNormal : vsiPosition;
  vec4 mp = modelMatrix * vec4(bpos, 1.0);
  vsoNormal = (transpose(inverse(modelMatrix)) * vec4(vsiNormal, 0.0)).xyz;
  vsoModPos = mp.xyz;
  gl_Position = projectionMatrix * viewMatrix * mp;
}
