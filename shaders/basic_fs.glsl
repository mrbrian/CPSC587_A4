#version 330 core

vec3 L = vec3(0, 1, -0.5);
uniform vec3 ambient = vec3(1, 1, 1);

in vec3 N;
in vec3 interpolateColor;

out vec3 color;

void main()
{
	L = normalize(L);
    color =  ambient * interpolateColor + max(dot(L, N), 0) * interpolateColor;
}
