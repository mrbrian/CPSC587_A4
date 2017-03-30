#version 330
layout( location = 0 ) in vec3 vert_modelSpace;
layout( location = 1 ) in vec3 vert_normal;

uniform mat4 MV;
uniform mat4 MVP;
uniform vec3 inputColor;

out vec3 N;
out vec3 interpolateColor;

void main()
{
    mat3 normalMatrix = mat3(MV);
    N = normalize(normalMatrix * vert_normal);		
	
	gl_Position = MVP * vec4( vert_modelSpace, 1.0 );	
		
	interpolateColor = inputColor;
}
