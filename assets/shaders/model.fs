#version 330

uniform vec4 lumPos;

uniform sampler2D myTexture;
uniform int hasTexture;
uniform vec4 diffuse_color;
uniform vec4 ambient_color;

in vec2 vsoTexCoord;
in vec3 vsoNormal;
in vec4 vsoModPosition;

out vec4 fragColor;

void main(void) {
  vec3 lum = normalize(vsoModPosition.xyz - lumPos.xyz);
  float diffuse = clamp(dot(normalize(vsoNormal), -lum), 0.1, 1.0);
  vec3 lightDirection = vec3(lumPos - vsoModPosition);

  vec4 diffuseReflection = ambient_color * 0.2 + diffuse_color * diffuse;
  fragColor = diffuseReflection;
  if (hasTexture != 0)
    fragColor *= texture(myTexture, vsoTexCoord);
}
