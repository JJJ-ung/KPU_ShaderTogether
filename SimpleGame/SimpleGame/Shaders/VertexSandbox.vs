#version 330

in vec3 a_Position;

uniform float u_Time;
const float c_PI = 3.141592;

out float v_Alpha;

void sinLine()
{
	vec4 newPosition = vec4(a_Position, 1.0);

	v_Alpha = 1.0 - (a_Position.x + 1.0) * 0.5f;

	float value = (a_Position.x + 1.f) * c_PI - u_Time;		// 0~2pi 사이의 값
	//newPosition.y  = sin(value) * mix(0.f, 1.f, (a_Position.x + 1.f) * 0.5f);

	float d = a_Position.x + 1.f;	// distance

	newPosition.y  = sin(value) * (d * 0.4);
	gl_Position = newPosition;
}

void main()
{
	sinLine();
}
