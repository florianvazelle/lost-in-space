#version 330

uniform sampler2D myTexture;
uniform int highlight;

in vec2 vsoTexCoord;

out vec4 fragColor;

void main(void) {
  vec4 c = texture(myTexture, vsoTexCoord);
  fragColor = vec4(c.rgb, length(c.rgb) > 0.0 ? 1.0 : 0.0);
  if (highlight == 0) {
    fragColor.rgb = fragColor.rgb * 0.75;
  }
}
