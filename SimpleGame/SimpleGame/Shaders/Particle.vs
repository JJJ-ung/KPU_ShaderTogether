#version 330

in vec3 a_Position;
in vec3 a_Speed;

uniform float u_Time;
uniform vec3 u_Accel;

const vec3 c_Vel = vec3(0.001, 0.0, 0.0);

void main()
{
	vec4 newPosition;
	newPosition.xyz = a_Position + a_Speed * u_Time + 0.5 * u_Accel * u_Time * u_Time;
	newPosition.w = 1;
	gl_Position = newPosition;
}