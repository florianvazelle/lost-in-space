#version 330

uniform sampler2D myTexture;
uniform vec4 lumPos;
uniform int light;
uniform int type;
uniform float getTime;

uniform vec3 vector_view;

in vec3 gsoNormal;
in vec3 gsoModPos;
in float gsoIdiffus;
in vec2 gsoTexCoord;
in float gsoColor;

out vec4 fragColor;

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

  float lat = gsoTexCoord.y * 180 - 90;
  float lon = gsoTexCoord.x * 360 - 90;
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

vec4 earth() {
  float color = generate_planet(1., false);
  vec3 color_rgb = (vec3(color, color, color) + 2.);
  vec3 blue = color_rgb * vec3(0.15, 0.05, 0.75);
  vec3 green = color_rgb * vec3(0.15, 0.75, 0.05);
  return vec4(mix(blue, green, color), 1);
}

vec4 asteroid() {
  float color = generate_planet(2., false);
  vec3 color_rgb = (vec3(color, color, color) + 2.);
  vec3 grey = color_rgb * vec3(0.15, 0.15, 0.15);
  return vec4(grey, 1);
}

vec4 sun(float color) {
  vec3 color_rgb = (vec3(color, color, color) + 2.);
  vec3 red = color_rgb * vec3(0.8, 0.35, 0.1);
  return vec4(red, 1);
}

void main(void) {
  if (light != 0) {
    if (type != 1) {
      const vec4 lum_diffus = vec4(1, 1, 0.9, 1);
      const vec4 lum_amb = vec4(0.8, 0.8, 1, 1);
      const vec4 lum_spec = vec4(1, 1, 0.75, 1);
      const float Iamb = 0.15;

      vec3 L = normalize(gsoModPos - lumPos.xyz);
      float Idiffuse = 0, Ispec = 0;
      vec4 color = vec4(1);
      vec3 N = normalize(gsoNormal);

      vec3 B = cross(normalize(vec3(N.x, 0, N.z)), vec3(0, 1, 0));
      vec3 T = cross(N, B);
      Idiffuse = clamp(dot(N, -L), 0, 1);

      if (type == 2)
        color = earth();
      if (type == 3)
        color = asteroid();
      fragColor = lum_diffus * color * Idiffuse + lum_amb * Iamb * color +
                  lum_spec * Ispec;
    } else {
      fragColor = sun(gsoColor);
    }
  } else {
    fragColor = texture(myTexture, gsoTexCoord);
  }
}
