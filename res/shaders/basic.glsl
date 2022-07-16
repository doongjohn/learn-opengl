#type_vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
out vec2 v_TexCoord;

uniform mat4 u_Mvp;

void main() {
  gl_Position = u_Mvp * position;
  v_TexCoord = texCoord;
}


#type_fragment
#version 460 core

out vec4 color;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
  vec4 texColor = texture(u_Texture, v_TexCoord);
  color = texColor;
}
