#version 330

uniform vec4 lumPos;
uniform vec3 vector_view;

uniform sampler2D myTexture;
uniform int hasTexture;

in vec2 vsoTexCoord;
in vec3 vsoNormal;
in vec4 vsoModPosition;

out vec4 fragColor;

void main(void) {
  const vec4 diffuse_color = vec4(0.5, 0.5, 0.45, 0.5);
  const vec4 ambient_color = vec4(0.4, 0.4, 0.5, 0.5);
  const vec4 specular_color = vec4(0.5, 0.5, 0.375, 0.5);

  vec3 lum = normalize(vsoModPosition.xyz - lumPos.xyz);
  float diffuse = clamp(dot(normalize(vsoNormal), -lum), 0.1, 1.0);
  vec3 lightDirection = vec3(lumPos - vsoModPosition);

  /* vecteur vue */
  vec3 V = vector_view;
  /* reflet de L par rapport à la normale au fragment */
  vec3 R = reflect(lum, vsoNormal);
  /* Intensité de speculaire */
  float Ispec = pow(clamp(dot(R, -V), 0, 1), 10);

  fragColor =
      ambient_color * 0.2 + diffuse_color * diffuse + specular_color * Ispec;
  if (hasTexture != 0)
    fragColor *= texture(myTexture, vsoTexCoord);
}
