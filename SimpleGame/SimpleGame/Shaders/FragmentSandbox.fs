#version 330

in vec4 v_Color;

layout(location=0) out vec4 FragColor;

in vec2 v_Texcoord;

uniform vec2 u_Point;
uniform vec2 u_Points[3];
uniform float u_Time;

const float c_PI = 3.141592;

void circle()
{
    // 동그래미 만들기
    // 반지름 길이보다 짧은놈들은 색칠하고 아닌놈은 밖에 있는거니까 안칠하기
    vec2 newValue = v_Texcoord - u_Point;        // 가운데 좌표가 0.5니까 0.0으로 맞춰주려고
    float d = length(newValue); // 벡터의 길이 계산해주는 놈
    if(d < 0.1f)
    {
        FragColor = vec4(v_Texcoord.xy, 0.0, 1.0);
    }
    else
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

void circles()
{
    // 동심원 맨들기
    vec2 newValue = v_Texcoord - u_Point;        // 가운데 좌표가 0.5니까 0.0으로 맞춰주려고
    float d = length(newValue); // 벡터의 길이 계산해주는 놈
    FragColor = vec4(sin(d * 30.f));
}

void radar()
{
    vec2 newValue = v_Texcoord - vec2(0.5, 0.5);
    float d = length(newValue);
    float ring_mask = ceil(sin(d * c_PI * 4 - u_Time) - 0.95);
    //FragColor = vec4(sin(d * c_PI * 4 - u_Time));

    float obj_mask = 0.f;
    for(int i = 0; i < 3; ++i)
    {
        vec2 tmp = v_Texcoord - u_Points[i];
        float d = length(tmp);
        if(d < 0.1)
        {
            obj_mask += 1.0;
        }
    }
    //FragColor = vec4(value);

    float res = ring_mask * obj_mask;
    FragColor = vec4(res);
}

void main()
{
    radar();
}
