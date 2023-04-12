#version 330

in vec3 a_Position;
in vec3 a_Speed;
in float a_EmitTime;
in float a_LifeTime;

uniform float u_Time;
uniform vec3 u_Accel;

const float c_PI = 3.141592;
//const vec3 c_Vel = vec3(0.5, 0.5, 0.0);
const vec3 c_Vel = vec3(0.0, -0.5, 0.0);

float c_Angle = 45.f;

in float a_Amp;
in float a_Period;

in float a_Value;


vec4 GraphSin()
{
	vec4 newPos = vec4(0, 0, 0, 1);

	float t = u_Time - a_EmitTime;

	if(t < 0.0)
	{
	}
	else
	{
		float newT = a_LifeTime * fract(t/a_LifeTime);
		newPos.x = a_Position.x + c_Vel.x * newT;
		newPos.y = a_Position.y + c_Vel.y * newT;

		// 회전시키는거 시험 ★★★★★★★★★★★★★★★★★★★★★★★★★★★★

		vec2 nVel = vec2(-c_Vel.y, c_Vel.y);		// 기준벡터
		nVel = normalize(nVel);

		newPos.xy = newPos.xy + nVel *  newT * a_Amp *
								sin((1.0 + newT) * a_Period * newT * c_PI * 2.f);

		//vec4 outPos = newPos;
		//outPos.x = newPos.x * cos(radians(c_Angle)) - newPos.y * sin(radians(c_Angle));
		//outPos.y = newPos.x * sin(radians(c_Angle)) + newPos.y * cos(radians(c_Angle));
		//
		//newPos = outPos;
	}

	return newPos;
}

vec4 GraphSin2()
{
	vec4 newPos = vec4(0, 0, 0, 1);
	float t = u_Time - a_EmitTime;

	if(t >= 0.0)
	{
		float newT = a_LifeTime * fract(t/a_LifeTime);

		float paraX = sin(a_Value * 2 * c_PI);
		float paraY = cos(a_Value * 2 * c_PI);

		newPos.x = a_Position.x + paraX + c_Vel.x * newT;
		newPos.y = a_Position.y + paraY + c_Vel.y * newT;
	   
		vec2 nVel = vec2(-c_Vel.y, c_Vel.y);		// 기준벡터
		nVel = normalize(nVel);

		newPos.xy = newPos.xy + nVel *  newT * a_Amp *
								sin((1.0 + newT) * a_Period * newT * c_PI * 2.f);
	}

	return newPos;
}

vec4 P1()
{
	vec4 newPosition = vec4(0.0, 0.0, 0.0, 1.0);

	float t = u_Time - a_EmitTime;

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

	return newPosition;
}


void main()
{
	gl_Position = GraphSin2();
	//gl_Position = P1();
}