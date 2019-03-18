#version 330

uniform samplerCube tex;
in vec3 vsoTexCoord;

out vec4 fragColor;

void main (void) {
    fragColor = texture(tex, vsoTexCoord);
}
