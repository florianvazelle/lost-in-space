#version 330

uniform vec4 lumPos;
uniform float explosion;

in vec3 vsoNormal[];
in vec3 vsoModPos[];
in vec2 vsoTexCoord[];
in float vsoColor[];
/* les sorties du geometry vers le fragment */
out vec3 gsoNormal;
out vec3 gsoModPos;
out float gsoIdiffus;
out vec2 gsoTexCoord;
out float gsoColor;

/* en entrée un triangle */
layout(triangles) in;
/* en sortie, une bande de triangles à 3 sommets (càd 1 triangle) */
layout(triangle_strip, max_vertices = 3) out;

float rand(vec3 co) {
  return fract(sin(dot(co, vec3(12.9898, 78.233, 45.5432))) * 43758.5453);
}

void main(void) {
  vec3 normal = vec3(0);
  if (gl_in.length() == 3) {
    vec3 v1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 v2 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 v_normal = normalize(cross(v1, v2));

    for (int i = 0; i < gl_in.length(); i++) {
      if (explosion > 0.0) {
        vec3 v_pos =
            gl_in[i].gl_Position.xyz +
            v_normal * (explosion * 100.) * rand(gl_in[0].gl_Position.xyz);
        gl_Position = vec4(v_pos, gl_in[i].gl_Position.w);
      } else {
        gl_Position = gl_in[i].gl_Position;
      }

      vec3 L = normalize(vsoModPos[i] - lumPos.xyz);
      gsoNormal = vsoNormal[i];
      gsoIdiffus = dot(vsoNormal[i], -L);
      gsoModPos = vsoModPos[i];
      gsoTexCoord = vsoTexCoord[i];
      gsoColor = vsoColor[i];
      EmitVertex();
    }
    EndPrimitive();
  }
}
