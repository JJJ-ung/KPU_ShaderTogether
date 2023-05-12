#version 330

layout(location=0) out vec4 FragColor;

in float v_Alpha;

uniform vec4 u_Color;

uniform float u_Time;

void main()
{
	//float alpha = (int(v_Alpha * 100) % 2) * v_Alpha;

	float alpha = 1.0 - v_Alpha;
	alpha *= 100.0;
	alpha -= u_Time;

	// 딱 끊어지는 점선
	//int lineMask = int(alpha) % 2;

	// 보드라운 점선
	float lineMask = sin(alpha);

	FragColor = vec4(1.0, 1.0, 1.0, v_Alpha * lineMask);
}
