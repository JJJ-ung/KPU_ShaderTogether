#version 330

in vec4 v_Color;
in vec2 v_UV;
layout(location=0) out vec4 FragColor;

const float PI = 3.141592;

uniform sampler2D u_Texture;

void Circle()
{
	vec2 newValue = v_UV - vec2(0.5f, 0.5f);
	float d = length(newValue);
	if(d<0.5f)
	{
		FragColor = v_Color;
	}
	else
	{
		FragColor = vec4(0);
	}
}

void Textured()
{
	vec4 result = texture(u_Texture, v_UV)*v_Color;
	FragColor =result;
}

void main()
{
	//FragColor = vec4(1.0);
	//FragColor = v_Color;
	//FragColor = vec4(v_UV,0,1);
	//Circle();
	//FragColor = v_Color;
	Textured();
}	
