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

uniform vec3 u_Color_tl;
uniform vec3 u_Color_bl;
uniform vec3 u_Color_tr;
uniform vec3 u_Color_br;

out vec4 color;

// https://stackoverflow.com/questions/60212615/how-to-properly-blend-colors-across-two-triangles-and-remove-diagonal-smear
void main() {
  vec3 l = mix(u_Color_bl, u_Color_tl, v_Uv.t);
  vec3 r = mix(u_Color_br, u_Color_tr, v_Uv.t);
  vec3 c = mix(l, r, v_Uv.s);
  color = vec4(c, 1f);
}
