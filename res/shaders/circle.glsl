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

out vec4 out_Color;

in vec2 v_Uv;
vec2 uv = v_Uv;
vec3 color = vec3(1, 1, 1);

void main() {
  uv -= 0.5; // center = 0, 0
  color *= smoothstep(0.02, 0.0, length(uv) - 0.3);
  out_Color = vec4(color, 1.0);
}
