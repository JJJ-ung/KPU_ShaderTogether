#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexSampler;

uniform vec2 u_XYReapeat;
in vec2 v_TexPos;

void P1()
{
	float x = v_TexPos.x;
	float y = 1.0 - abs(v_TexPos.y* 2.0- 1.0);
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler, newTexPos);
}

void P2()
{
	float x = fract(v_TexPos.x * 3.0f);
	float dy = v_TexPos.y/3.0;
	float y = floor(3.0*(1.0 - v_TexPos.x))/3.0 + dy;
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler, newTexPos);
}

void P3()
{
	float x = fract(v_TexPos.x * 3.0f);
	float dy = v_TexPos.y/3.0;
	float y = floor(3.0*(v_TexPos.x))/3.0 + dy;
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler, newTexPos);
}

void P4()
{
	float x = v_TexPos.x;
	float dy = fract(v_TexPos.y*3.0)/3.0;  // float dy = v_TexPos.y/3.0; ->�긦 ���蹮���� ������ ����
	float y = floor(3.0*(1.0-v_TexPos.y))/3.0 + dy;
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler, newTexPos);
}

void P5()
{
	float dx = v_TexPos.x *u_XYReapeat.x;
	float x = dx + floor((1.0-v_TexPos.y)*u_XYReapeat.y)* 0.5;
	float y = fract(v_TexPos.y*u_XYReapeat.y);
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler,newTexPos);
}

void P6()
{
	float dy = v_TexPos.y *u_XYReapeat.y;
	float x = fract(v_TexPos.x*u_XYReapeat.x);
	float y = fract(dy + floor(v_TexPos.x*u_XYReapeat.x) * 0.5); // 0.5�� �󸶳� �и����̳��̴�.
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler,newTexPos);
}

void P7()
{
	float x = fract(v_TexPos.x+(1.0-v_TexPos.y));  ///-->�������� �� �����̴�#######################################
	float y = fract(v_TexPos.x+ v_TexPos.y);
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler,newTexPos);
}

void main()
{
	P7();
	
}

//float x = v_TexPos.x;
	//float y= v_TexPos.y;
	//���� 1
	//float x = v_TexPos.x;
	//float y = 1.0 - abs(v_TexPos.y * 2.0 -1.0);
	//vec2 newTexPos = vec2(x,y); 
	//���� 2
	//x = v_TexPos.x*3.0;
	//if(x < 1)
	//{
	//	y = (v_TexPos.y/3.0f);
	//}
	//else if(x < 2)
	//{
	//	y = (v_TexPos.y/3.0f)+(1.0f/3.0f)+0.005;
	//}
	//else
	//{
	//	y = (v_TexPos.y/3.0f)+(2.0f/3.0f)+0.005;
	//}
	//���� 3
	//x = v_TexPos.x*3.0;
	//if(x < 1)
	//{
	//	y = (v_TexPos.y/3.0f)+(2.0f/3.0f)+0.005;
	//}
	//else if(x < 2)
	//{
	//	y = (v_TexPos.y/3.0f)+(1.0f/3.0f)+0.005;
	//}
	//else
	//{
	//	y = (v_TexPos.y/3.0f);
	//}
	//���� 4
	//if(y < (1.0f/3.0f))
	//{
	//	y = (y + 2.0f/ 3.0f);
	//}
	//else if( y > (2.0f/3.0f))
	//{
	//	y = (y + 1.0f/ 3.0f);
	//}
	//���� 5
	//x = v_TexPos.x*2.0;
	//y = v_TexPos.y*2.0;
	//
	//if(y < 1)
	//{
	//	x = x+0.5;
	//}
	//���� 6
	//x = v_TexPos.x*2.0;
	//y = v_TexPos.y*2.0;
	//
	//if(x > 1)
	//{
	//	y = y+0.5;
	//}
	//vec2 newTexPos = vec2(x,y);
	//
	//FragColor = texture(u_TexSampler,newTexPos);