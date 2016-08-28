#version 300 es

//(*)Atencion: La divicion por 1.xx es un parche, porque al hacer el filtro de blur se perdian
//diferencias de depth. Podria verse luego como mejorarlo.

in vec4 positionToLight;

out vec2 ShadowData;

void main()
{	
	vec3 position = vec3(positionToLight);
	float depth = position.z / 1.0f;  //(*)Ver comentario arriba

	//Guardo el depth y depth*depth para el variance shadow map
	ShadowData = vec2(depth, depth*depth);
}
