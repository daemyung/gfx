precision mediump float;

layout(location = 0) in vec3 color;

layout(location = 0) out vec4 frag_color0;

void main()
{
	frag_color0 = vec4(color, 1.0);
}