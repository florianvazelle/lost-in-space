#version 330

uniform sampler2D myTexture;
uniform vec4 lumPos;
uniform int light;
uniform int type;

in vec3 gsoNormal;
in vec3 gsoModPos;
in float gsoIdiffus;
in vec2 gsoTexCoord;

out vec4 fragColor;

void main(void) {
  if (light != 0) {
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

    color = texture(myTexture, gsoTexCoord);
    if (type != 1) {
      fragColor = lum_diffus * color * Idiffuse + lum_amb * Iamb * color +
                  lum_spec * Ispec;
    } else {
      fragColor = color + lum_amb * Iamb * color + lum_spec * Ispec;
    }
  } else {
    fragColor = texture(myTexture, gsoTexCoord);
  }
}
