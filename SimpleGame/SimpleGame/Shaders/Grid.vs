#version 330

in vec3 a_Position;

out float v_greyScale;
out vec2 v_TexPos;

uniform float u_Time;
uniform vec2 u_TargetPos;

const float PI = 3.141592;

void main()
{
    float tx = a_Position.x + 0.5;
    float ty = 1.0 - (a_Position.y + 0.5);
    v_TexPos = vec2(tx, ty);
    float x = a_Position.x;
    float value = (a_Position.x+0.5) * 2.0 * PI - u_Time;
    float temp = a_Position.x+0.5;
    float y = a_Position.y + 0.25 * temp * sin(value);

    vec3 targetPos = vec3(u_TargetPos, 0.0);

    float timeY = (a_Position.y +0.5);
    float newTime = u_Time - timeY*3.0;
    newTime = max(0,newTime);

    vec3 morphPos =  mix(vec3(x, y, 0.0), targetPos, newTime);

    v_greyScale = (sin(value)+1.0) / 2.0;
    gl_Position = vec4(morphPos,1.0f);
}
