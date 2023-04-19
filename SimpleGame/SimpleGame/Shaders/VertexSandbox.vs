#version 330

in vec3 a_Position;

uniform float u_Time;

const float c_PI = 3.141592;

void main()
{
	vec4 newPosition = vec4(a_Position, 1.0);

	float value = (a_Position.x + 1.f) * c_PI - u_Time;		// 0~2pi 사이의 값
	newPosition.y  = sin(value) * a_Position.x;

	gl_Position = newPosition;
}
