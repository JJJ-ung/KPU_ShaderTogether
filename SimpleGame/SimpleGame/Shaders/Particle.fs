#version 330

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_Texcoord;

void circle()
{
    // ���׷��� �����
    // ������ ���̺��� ª������� ��ĥ�ϰ� �ƴѳ��� �ۿ� �ִ°Ŵϱ� ��ĥ�ϱ�
    vec2 newValue = v_Texcoord - vec2(0.5, 0.5);        // ��� ��ǥ�� 0.5�ϱ� 0.0���� �����ַ���
    float d = length(newValue); // ������ ���� ������ִ� ��
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
