#version 330
in vec3 pos;
in vec2 uv;

out vec4 frag_color;

uniform sampler2D tex;

void main()
{
	//frag_color = vec4(uv, 0, 1);
	vec3 col = texture(tex, uv).rgb;
	frag_color = vec4(col, 1.0);
}
