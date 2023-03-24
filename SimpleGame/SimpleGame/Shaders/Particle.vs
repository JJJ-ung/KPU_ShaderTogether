#version 330

in vec3 a_Position;
in vec3 a_Speed;
in float a_EmitTime;
in float a_LifeTime;

uniform float u_Time;
uniform vec3 u_Accel;

const float PI = 3.141592;
const vec3 c_Vel = vec3(1.0, 0.0, 0.0);

void main()
{
	vec4 newPos = vec4(0, 0, 0, 1);

	float t = fract(u_Time);

	newPos.x = a_Position.x + c_Vel.x * t;

	// 이거 시험 문제 **
	newPos.y = a_Position.y + sin(t * PI * 2.f);

	gl_Position = newPos;
}