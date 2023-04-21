#version 330

layout(location=0) out vec4 FragColor;

uniform vec4 u_Color;
uniform float u_Time;
in float v_Alpha;
void main()
{
	float newAlpha = sin(v_Alpha * 200);
	float newV = 1.0 - v_Alpha;
	newV *= 300.0;
	newV += u_Time * 3;
	int lineMask = int(newV)%2;
	float alphaMask = sin(newV);
	FragColor = vec4(1.0,1.0,1.0,v_Alpha * alphaMask);
	
}
