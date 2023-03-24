#version 330

in vec3 a_Position;
in vec3 a_Speed;
in float a_EmitTime;
in float a_LifeTime;

uniform float u_Time;
uniform vec3 u_Accel;

void main()
{
	vec4 newPosition = vec4(0.0, 0.0, 0.0, 1.0);

	float t = u_Time - a_EmitTime;	// 파티클별로 다른 시간

	if(t < 0.0)
	{
	}
	else
	{
		float newT = a_LifeTime * fract(t/a_LifeTime);
		newPosition.xyz = a_Position + a_Speed * newT 
												+ 0.5 * u_Accel * newT * newT;
		newPosition.w = 1;
	}

	gl_Position = newPosition;
}