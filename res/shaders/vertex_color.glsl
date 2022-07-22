#type_vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
out vec4 v_Color;

uniform mat4 u_Mvp;

void main() {
  gl_Position = u_Mvp * position;
  v_Color = color;
}


#type_fragment
#version 460 core

in vec4 v_Color;
out vec4 out_Color;

void main() {
  out_Color = v_Color;
}
