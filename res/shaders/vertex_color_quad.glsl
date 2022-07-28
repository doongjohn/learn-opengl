#type_vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

uniform mat4 u_Mvp;

out vec2 v_Uv;

void main() {
  gl_Position = u_Mvp * position;
  v_Uv = uv;
}


#type_fragment
#version 460 core

in vec2 v_Uv;

uniform vec3 u_Color_01; // top left
uniform vec3 u_Color_00; // bottom left
uniform vec3 u_Color_11; // top right
uniform vec3 u_Color_10; // bottom right

out vec4 out_Color;

// defualt linear color interpolation only works with triangle
// so we need a custom shader for a quad in order to properly interpolate its color
// https://stackoverflow.com/questions/60212615/how-to-properly-blend-colors-across-two-triangles-and-remove-diagonal-smear
void main() {
  // bilinear interpolation
  vec3 l = mix(u_Color_00, u_Color_01, v_Uv.t);
  vec3 r = mix(u_Color_10, u_Color_11, v_Uv.t);
  vec3 c = mix(l, r, v_Uv.s);
  out_Color = vec4(c, 1f);
}
