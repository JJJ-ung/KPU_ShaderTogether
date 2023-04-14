#version 330

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_Texcoord;

void circle()
{
    // 동그래미 만들기
    // 반지름 길이보다 짧은놈들은 색칠하고 아닌놈은 밖에 있는거니까 안칠하기
    vec2 newValue = v_Texcoord - vec2(0.5, 0.5);        // 가운데 좌표가 0.5니까 0.0으로 맞춰주려고
    float d = length(newValue); // 벡터의 길이 계산해주는 놈
    if(d < 0.5f)
    {
        FragColor = v_Color;
    }
    else
    {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}

void main()
{
	//FragColor = v_Color;

    circle();
}
