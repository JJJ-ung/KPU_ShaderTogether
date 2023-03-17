#version 330

// 각 쉐이더별로 입력값은 [in]이라는 모디파이어를 붙임

in vec3 a_Position;

in vec3 a_Color;

	//외부에서 들어오는 입력값 (사용자가 정의한)
// layout(location = 0) in vec3 a_Position;
	// 인덱스 내가 직접 지정하는 방법
	// 이렇게 하지 않으면 자동으로 인덱스가 부여됨
// in vec2 a_Texcoord;

uniform vec4 u_Trans;

uniform float u_Scale;

void main()
{
	vec4 newPosition;
	newPosition.xy = u_Scale * (a_Position.xy)*u_Trans.w + u_Trans.xy;
	newPosition.z = 0;
	newPosition.w= 1;
	gl_Position = newPosition;
}
