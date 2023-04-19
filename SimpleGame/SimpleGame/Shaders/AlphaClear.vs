#version 330

in vec3 a_Position;
in vec3 a_Position1;
//in vec2 a_Texcoord;
uniform vec4 u_Trans;

uniform float u_Scale;


void main()
{
	vec4 newPosition = vec4(a_Position, 1.f);
	gl_Position = newPosition;
}
