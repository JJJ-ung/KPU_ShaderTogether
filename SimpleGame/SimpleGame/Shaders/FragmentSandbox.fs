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
    // ���׷��� �����
    // ������ ���̺��� ª������� ��ĥ�ϰ� �ƴѳ��� �ۿ� �ִ°Ŵϱ� ��ĥ�ϱ�
    vec2 newValue = v_Texcoord - u_Point;        // ��� ��ǥ�� 0.5�ϱ� 0.0���� �����ַ���
    float d = length(newValue); // ������ ���� ������ִ� ��
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
    // ���ɿ� �ǵ��
    vec2 newValue = v_Texcoord - u_Point;        // ��� ��ǥ�� 0.5�ϱ� 0.0���� �����ַ���
    float d = length(newValue); // ������ ���� ������ִ� ��
    FragColor = vec4(sin(d * 30.f));
}

void radar()
{
    vec2 newValue = v_Texcoord - vec2(0.5, 1.0);
    float d = length(newValue);
    float value = sin(2 * d * c_PI - 13 *  u_Time) - 0.9;
    float ring_mask = ceil(value);

    float obj_mask = 0.f;
    for(int i = 0; i < 3; ++i)
    {
        vec2 tmp = v_Texcoord - u_Points[i];
        float d = length(tmp);
        if(d < 0.03)
        {
            obj_mask += 1.0;
        }
    }

    float res = ring_mask * obj_mask + 10 * value;
    FragColor = vec4(res);
}

void main()
{
    radar();
}