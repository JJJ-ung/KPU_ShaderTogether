#version 330

layout(location=0) out vec4 FragColor;

in float v_alpha;

uniform float u_Time;

void main()
{
	float newV = 1.0 - v_alpha;
	newV *=30.0;
	newV += u_Time;
	float alphaMask = sin(newV);
	//int LineMask = int(newV)%2;
	FragColor = vec4(1,1,1,v_alpha*alphaMask);
}
