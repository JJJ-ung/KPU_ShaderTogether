#version 330

layout(location=5) out vec4 FragColor;

layout(location=0) out vec4 FragColor0;
layout(location=1) out vec4 FragColor1;
layout(location=2) out vec4 FragColor2;
layout(location=3) out vec4 FragColor3;
layout(location=4) out vec4 FragColor4;

uniform vec4 u_Color;
uniform vec2 u_point;
uniform vec2 u_points[3];
uniform float u_Time;
uniform sampler2D u_Texture;

in vec2 v_UV;

const float PI = 3.141592;

void Circle()
{
	vec2 newValue = v_UV - u_points[1];
	float d = length(newValue);
	if(d<0.1f)
	{
		FragColor = vec4(v_UV.xy,0.0,1.0);
	}
	else
	{
		FragColor = vec4(0);
	}
}

void Circles()
{
	vec2 newValue = v_UV - u_points[1];
	float d = length(newValue);
	FragColor4 = vec4(sin(10*d*PI));
}

void radar()
{
	vec2 newValue = v_UV - vec2(0.5,0.5);
	float d = length(newValue);
	float value = sin(2*d*PI -7* u_Time)-0.9;
	float ring_mask = ceil(value);

	float obj_mask = 0.0f;

	for(int i=0; i<3; ++i)
	{
		vec2 temp = v_UV - u_points[i];
		float d= length(temp);
		if(d<0.03f)
		{
			obj_mask +=1.0;
		}
	}
	FragColor2 = vec4(ring_mask*obj_mask+10*value);
}

void UVTest()
{
	FragColor = vec4(0);
	float powValue=100;
	float  sinResultX = pow(sin(v_UV.x * 10 * PI),powValue);
	float  sinResultY = pow(sin(v_UV.y * 10 * PI),powValue);
	float finalResult = max(sinResultX,sinResultY);
	FragColor0 = vec4(finalResult);
}

void sinGraph()
{
	FragColor3 = vec4(0);
	for(int i=0; i<5; ++i)
	{
		float newTime = u_Time + i * 0.2;
		vec2 newUV = vec2(v_UV.x, 2.0*(v_UV.y-0.5));
		float newInput = v_UV.x * 2 * PI+PI;
		float sinValue = v_UV.x * sin(newInput-newTime*5);
		float width = 0.005;
		float newAlpha = 1.0-v_UV.x;
		float newLines = sin(v_UV.x*200.0-newTime*50);
		if(sinValue < newUV.y && sinValue+width > newUV.y) FragColor3 += vec4(1.0*newAlpha*newLines);
	}
}

void testgraph()
{
	vec2 newValue = v_UV - u_points[1];
	float d = length(newValue);
	if(d<0.5f)
	{
		FragColor = vec4(v_UV.xy,0.0,1.0);
	}
	else
	{
		FragColor = vec4(0);
	}
}

void RealFlag()
{
	float period = (v_UV.x + 1.0) * 1.0;
	float xValue = v_UV.x*2.0*PI * period;
	float yValue = ((1.0-v_UV.y)-0.5)*2.0;
	float sinValue = 0.25*sin(xValue -10.0* u_Time);
	if(sinValue*v_UV.x + 0.75 > yValue && sinValue*v_UV.x - 0.75 < yValue)
	{
		float vX = v_UV.x;
		float yWidth = 1.5;// sinValue*v_UV.x + 0.75 - sinValue*v_UV.x - 0.75
		float yDistance = yValue - (sinValue*v_UV.x - 0.75);
		float vY = 1.0-yDistance / yWidth;
		FragColor1 = texture(u_Texture , vec2(vX,vY));
		//FragColor = vec4(vX, vY, 0 ,1);
	}
	else
	{
		FragColor1 = vec4(0);
	}
}


void main()
{
	UVTest(); //->0
	RealFlag(); //->1
	radar(); // ->2
	sinGraph(); //->3
	Circles(); // ->4
}