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

vec3 color1 = vec3(179, 255, 223) / 255.0;
vec3 color2 = vec3(255, 145, 255) / 255.0;
float r = 0.4;

void main() {
  uv -= 0.5;

  float len = length(uv);
  float alpha = smoothstep(0.01, 0.0, len - r);

  float gradient = smoothstep(r, 0.0, len);
  vec3 c = color1 * (1 - gradient) + color2 * gradient;

  out_Color = vec4(c, alpha);
}





// in vec2 v_Uv;
// vec2 uv = v_Uv;
// vec4 color = vec4(0.2, 0.2, 1.0, 1.0);

// void main() {
//   uv -= 0.5; // make center (0, 0)

//   float fade = 0.02;
//   float d = length(uv);
//   float alphaMask =
//     smoothstep(fade, 0.0, d - 0.3) *
//     smoothstep(0.0, fade, d - 0.2);

//   color.w *= alphaMask;
//   out_Color = color;
// }
