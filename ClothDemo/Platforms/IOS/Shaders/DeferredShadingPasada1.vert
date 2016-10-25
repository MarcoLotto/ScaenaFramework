#version 420

in vec3 VertexPosition; 
in vec3 VertexNormal; 
in vec2 VertexTexCoord; 

out vec3 posicion;
out vec3 normal;
out vec2 TexCoord;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 perspMatrix;
uniform mat3 normalMatrix;

void main()
{
	normal = normalize(normalMatrix * VertexNormal);
	posicion = vec3(viewMatrix * modelMatrix * vec4(VertexPosition, 1.0f));	
	TexCoord = VertexTexCoord;

	gl_Position = perspMatrix * vec4(posicion, 1.0f);
}
	