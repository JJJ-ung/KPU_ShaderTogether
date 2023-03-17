#version 330

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
	// 보간 + 래스터라이징 되어서 가변된 값이 넘어온다

void main()
{
	//FragColor = v_Color;
	FragColor = vec4(1.f);
}
