#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 TexCoords;

uniform vec2 pos;

void main()
{
    gl_Position = vec4(vertex.xy + pos, 0.0, 1.0);
    TexCoords = vertex.zw;
}  
