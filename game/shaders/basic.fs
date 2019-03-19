#version 330
uniform sampler2D tex;
uniform int border;
uniform int alpha;
float cutoff = 0;

in  vec2 vsoTexCoord;
out vec4 fragColor;

void main(void) {
  if( border != 0 && (vsoTexCoord.s < 0.02 ||
		      vsoTexCoord.t < 0.02 ||
		      (1 - vsoTexCoord.s) < 0.02 ||
		      (1 - vsoTexCoord.t) < 0.02 ) )
    fragColor = vec4(0.5, 0, 0, 1);
  else
    fragColor = texture(tex, vsoTexCoord);

    if (gl_FragColor.a < cutoff)
        // alpha value less than user-specified threshold?
    {
        discard; // yes: discard this fragment
    }
}
