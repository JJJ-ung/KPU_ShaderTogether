#version 330

in vec4 a_Color;
in vec3 a_Position;

uniform float u_Scale;

//out vec4 v_Color;

void main()
{
	vec4 newPosition;
	newPosition.xyz = a_Position;
	newPosition.w = 1;
	gl_Position = newPosition;

	
	//v_Color = a_Color;
}
