#version 420

in vec3 VertexPosition;

uniform mat4 MVPmatrix;

void main(){
	gl_Position = MVPmatrix * vec4(VertexPosition, 1.0f);
}
