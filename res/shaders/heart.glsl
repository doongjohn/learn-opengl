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
vec4 color = vec4(1.0, 0.2, 0.2, 1.0);

void main() {
  uv -= 0.5; // make center (0, 0)

  uv.x *= 0.85;
  uv.y -= sqrt(smoothstep(0.0, 1.0, abs(uv.x))) * 0.38;
  float d = length(uv);
  float radius = 0.3;
  float fade = 0.02;
  float alphaMask = smoothstep(radius + fade, radius - fade, d);

  color.w *= alphaMask;
  out_Color = color;
}
