#type_vertex
#version 460 core

layout(location = 0) in vec4 position;

uniform mat4 u_Mvp;

void main() {
  gl_Position = u_Mvp * position;
}


#type_fragment
#version 460 core

out vec4 out_Color;

uniform vec4 u_Color;

void main() {
  out_Color = u_Color;
}
