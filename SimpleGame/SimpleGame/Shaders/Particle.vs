#version 330

in vec3 a_Position;
in vec3 a_Vel;
in float a_emit_time;
in float a_life_time;
in float a_Period;
in float a_Amp;
in float a_Value;

in vec4 a_Color;
in vec2 a_UV;

uniform float u_Time;
uniform vec3 u_Accel;

const vec3 c_Vel = vec3(0.0,-0.8,0.0);

const float PI = 3.141592;
//const float c_Amp = 0.5;
//const float c_Period = 2.0;

out vec4 v_Color;
out vec2 v_UV;

vec4 GraphSin()
{
	vec4 newPos = vec4(0,0,0,1);

	float t = u_Time - a_emit_time;
	float alpha=1.0f;

	if(t<0.0)
	{
	}
	else
	{
		float newT = a_life_time*fract(t/a_life_time);

		float paraX = sin(a_Value * 2.0 * PI);
		float paraY = cos(a_Value * 2.0 * PI);

		newPos.x = a_Position.x + paraX + c_Vel.x * newT;
		newPos.y = a_Position.y + paraY + c_Vel.y * newT;
		vec2 nVel = vec2(-c_Vel.y , c_Vel.x);
		nVel = normalize(nVel);
		newPos.xy=newPos.xy + nVel * newT*a_Amp*sin((1.0+newT)*a_Period * newT * 2.0 * PI);

		//newPos.y = a_Position.y +  c_Vel.y * newT+ a_Amp * newT * sin((1.0+newT)*a_Period * newT * 2.0 * PI);
		alpha = 1.0 - newT / a_life_time;
		alpha = pow(alpha,2.0f);
	}
	v_Color= vec4(a_Color.rgb,a_Color.a*alpha);
	return newPos;
}

vec4 P1()
{
	vec4 newPosition=vec4(0,0,0,1);
	float t = u_Time - a_emit_time;
	if(t>0.0)
	{
		float newT = fract(t/a_life_time) * a_life_time;
		newPosition.xyz = a_Position + a_Vel * newT + 0.5f * u_Accel* (newT* newT);
		newPosition.w= 1;
	}
	return newPosition;
}

void main()
{
	gl_Position = GraphSin();
	v_UV = a_UV;
}

//void main() // ºÐ¼ö´ë
//{
//	vec4 newPosition=vec4(0,0,0,1);
//	float t = u_Time - a_emit_time;
//	if(t>0.0)
//	{
//		float newT = fract(t/a_life_time) * a_life_time;
//		newPosition.xyz = a_Position + a_Vel * newT + 0.5f * u_Accel* (newT* newT);
//		newPosition.w= 1;
//	}
//
//	//newPosition.x= a_Position.x + u_Time;
//	//newPosition.yz=a_Position.yz;
//
//	gl_Position = newPosition;
//}