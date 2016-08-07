#version 420

in vec3 posicion;
in vec3 normal;
in vec2 TexCoord;
in mat3 toObjectLocal;
in mat3 normMatrix;

uniform sampler2D diffuseMap; 
uniform sampler2D displacementMap; 
uniform sampler2D normalMap; 

uniform vec2 uvTranslation;  // Transformaciones de la coordenadas UV
uniform vec2 uvScale;

uniform float normalFactor;
uniform float displacementFactor;
uniform float atenuationFactor;

out vec3 NormalData;
out vec3 ColorData;
out vec3 MaterialData;

 struct Material{
	float Ka;
	float Kd;
	float Ks;
	float Kb;
};
uniform Material mat;


vec2 ParallaxTexCoord(vec2 textureCoord){
	float s = displacementFactor * atenuationFactor;
	float maxIt = 6.0f;
	float minIt = 4.0f;
	float farDist = 0.6f;
	float h = 0.0f;
	vec3 hP = vec3(0.0f);
		
	//Encuentro el vector de vista en coordenadas locales	
	vec3 dir = normalize(toObjectLocal * posicion);
	vec3 n = vec3(0.0f, 0.0f, 1.0f);

	//Hago un primer fetch
	vec3 v = vec3(textureCoord, 0.0f) - (farDist)*dir * s;
	h= texture(displacementMap, vec2(v)).x - 0.5f;
	vec3 hPAnt = vec3(v.x, v.y, s*h);
	
	//Encuentro muestras de altura de varios puntos y calculo un promedio
	maxIt = minIt + maxIt - maxIt*dot(-n, dir)/2.0f; 
	for(int i = 1; i <= maxIt ; i++){		
		float alpha = i*farDist/maxIt;
		v = vec3(textureCoord, 0.0f) - (farDist - alpha)*dir * s;
		h= texture(displacementMap, vec2(v)).x - 0.5f;
		hP = vec3(v.x, v.y, h);
		if(h >=  v.z){			
			vec2 m1 = vec2((hPAnt.z - hP.z)/(hPAnt.x-hP.x), (hPAnt.z - hP.z)/(hPAnt.y-hP.y));
			vec2 m2 = vec2(dir.z/dir.x, dir.z/dir.y);
			float xCoord = (hPAnt.z - hPAnt.x * m1.x + textureCoord.x * m2.x)/(m2.x-m1.x); 
			float yCoord = (hPAnt.z - hPAnt.y * m1.y + textureCoord.y * m2.y)/(m2.y-m1.y);
			return vec2(xCoord,  yCoord);
		}		
		hPAnt = hP;
	}	
	return textureCoord - (h) * vec2(dir) *s;
}


void main()
{	
	//De aqui en mas se toman las coordenadas del parallax
	vec2 FixedTexCoord = ParallaxTexCoord(uvScale * (TexCoord + uvTranslation));
		
	NormalData = normalize(vec3(texture( normalMap, FixedTexCoord )-vec4(0.5f, 0.5f, 0.5f, 0.0f)) * toObjectLocal) * normalFactor;	
	ColorData = vec3(texture(diffuseMap, FixedTexCoord));
	MaterialData.x = mat.Ka;
	MaterialData.y = mat.Kd;
	MaterialData.z = mat.Ks;	
}