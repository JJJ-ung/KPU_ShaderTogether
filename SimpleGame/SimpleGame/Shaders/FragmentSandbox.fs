#version 330

in vec4 v_Color;

layout(location=0) out vec4 FragColor;

varying vec2 v_Texcoord;


void circle()
{
    // ���׷��� �����
    // ������ ���̺��� ª������� ��ĥ�ϰ� �ƴѳ��� �ۿ� �ִ°Ŵϱ� ��ĥ�ϱ�
    vec2 newValue = v_Texcoord - vec2(0.5, 0.5);        // ��� ��ǥ�� 0.5�ϱ� 0.0���� �����ַ���
    float d = length(newValue); // ������ ���� ������ִ� ��
    if(d < 0.5f)
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
    vec2 newValue = v_Texcoord - vec2(0.5, 0.5);        // ��� ��ǥ�� 0.5�ϱ� 0.0���� �����ַ���
    float d = length(newValue); // ������ ���� ������ִ� ��
    FragColor = vec4(sin(d * 30.f));
}

void main()
{
    circles();
}
