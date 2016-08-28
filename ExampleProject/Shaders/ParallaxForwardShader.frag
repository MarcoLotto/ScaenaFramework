#version 420

in vec3 posicion;
in vec2 TexCoord;
in mat3 toObjectLocal;

out vec4 FragmentData;

//*******************************************************************
//	Variables del shader particular
//*******************************************************************

uniform sampler2D diffuseMap; 
uniform sampler2D displacementMap; 
uniform sampler2D normalMap; 

uniform vec2 uvTranslation;  // Transformaciones de la coordenadas UV
uniform vec2 uvScale;

uniform float normalFactor;
uniform float displacementFactor;
uniform float atenuationFactor;

 struct Material{
	float Ka;
	float Kd;
	float Ks;
	float Kb;
};
uniform Material mat;

//*******************************************************************
//	Variables de iluminacion
//*******************************************************************

struct Luz{	
	vec4 posicion;	
	vec3 intensidad;
	vec3 direccion;
	float spotFactor;

	int shadowIndex;  //El indice de shadowMap que le corresponde (-1 si ninguno)			
};

uniform Luz luz[50];
uniform int cantLucesActivas;

uniform float gamma;  //Correcion gamma

//Parametros de las sombras, usar el mismo indice para ambos!
uniform mat4 shadowMatrix0;  
uniform sampler2D shadowMap0;
uniform mat4 shadowMatrix1;  
uniform sampler2D shadowMap1;
uniform mat4 shadowMatrix2;  
uniform sampler2D shadowMap2;

//*******************************************************************
//	Funciones de iluminacion
//*******************************************************************

vec3 phongPuntual(vec3 norm, Luz luz, vec3 posicion, Material mat, float sombreado){
		
	vec3 LightDir = normalize( luz.posicion.xyz - posicion );
			
	//Calculo el factor de decaimiento
	float b = 0.05f;
	float c = 1.0f;
	float decaimiento = (luz.intensidad.x+luz.intensidad.y+luz.intensidad.z)/3.0f * (1/(b * length(luz.posicion.xyz - posicion) + c));
			
	//vec3 v = normalize(vec3(-posicion));
	//vec3 r = normalize( v + LightDir );
	//return luz[i].intensidad*(mat.Ka + decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0f))) + mat.Ks*pow(decaimiento, 1.4f)*(pow( max(dot(r,norm), 0.0 ), mat.Kb)));	
	vec3 r = reflect( -LightDir, norm );
	vec3 v = normalize(vec3(-posicion));		
	return luz.intensidad*(sombreado*decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0f))) + sombreado*mat.Ks*pow(decaimiento, 1.4f)*(pow( max(dot(r,v), 0.0 ), mat.Kb)));
}	

vec3 phongPuntualSoloDifusa(vec3 norm, Luz luz, vec3 posicion, Material mat, float sombreado){
		
	vec3 LightDir = normalize( luz.posicion.xyz - posicion );
			
	//Calculo el factor de decaimiento
	float b = 0.05f;
	float c = 1.0f;
	float decaimiento = (luz.intensidad.x+luz.intensidad.y+luz.intensidad.z)/3.0f * (1/(b * length(luz.posicion.xyz - posicion) + c));			
	
	vec3 v = normalize(vec3(-posicion));		
	return luz.intensidad*(sombreado*decaimiento*(mat.Kd*max(dot(LightDir,norm), 0.0f)));
}	

vec3 phongDireccional(vec3 norm, Luz luz, vec3 posicion, Material mat, float sombreado){
		
	vec3 LightDir = -normalize( luz.direccion );
	
	vec3 r = reflect( -LightDir, norm );
	vec3 v = normalize(vec3(-posicion));		
	return luz.intensidad*(sombreado*(mat.Kd*(max(dot(LightDir,norm), 0.0f)) + mat.Ks*(pow( max(dot(r,v), 0.0 ), mat.Kb))));
}	

vec3 phongSpot(vec3 norm, Luz luz, vec3 posicion, Material mat, float sombreado){
	
	//Primero planteo como si fuese una luz puntual o spot
	vec3 LightDir = normalize( luz.posicion.xyz - posicion );

	//Calculo el decaimiento radial
	float decaimSpot = max(1.0f - dot(LightDir,  luz.direccion), 0.0f);		
	decaimSpot = pow(decaimSpot, luz.spotFactor); 
	
	//Calculo el factor de decaimiento
	float b = 0.05f;
	float c = 1.0f;
	float decaimiento = (luz.intensidad.x+luz.intensidad.y+luz.intensidad.z)/3.0f * (1/(b * length(luz.posicion.xyz - posicion) + c));
	decaimiento *= decaimSpot;

	// Calculo phong, dividiendo la intensidad por el factor de spot (sino se desproporciona la intensidad con factores muy grandes)
	vec3 r = reflect( -LightDir, norm );
	vec3 v = normalize(vec3(-posicion));
	vec3 finalIntensity = luz.intensidad / luz.spotFactor;
	return finalIntensity*(sombreado*decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0f))) + sombreado*sombreado*mat.Ks*pow(decaimiento, 1.4f)*(pow( max(dot(r,v), 0.0 ), mat.Kb)));
}	

//*******************************************************************
//	Funciones de sombras
//*******************************************************************

float ReduceLightBleeding(float p_max, float amount)
{
	return clamp((p_max - amount) / (1 - amount), 0, 1);
}

float chebyshevUpperBound(vec2 moments, float t)
{
	float min_variance = 0.07f;

	// One-tailed inequality valid if t > moments.x
    float p = 0.0f;
	if(t <= moments.x){
		p = 1.0f;
	}

	// Compute variance
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance, min_variance);

	// Compute probabilistic upper bound.
	float d = t - moments.x;
	float p_max = variance / (variance + d*d);

	//Se reduce el light bleeding, variar el segundo parametro para mayor o menor efecto
	//p_max = ReduceLightBleeding(p_max, 0.2f);

	return max(p, p_max);
}

float shadowContribution(sampler2D shadowMap, vec4 shadowCoord)
{
	shadowCoord.x /= shadowCoord.w;
	shadowCoord.y /= shadowCoord.w;
	//shadowCoord.z /= shadowCoord.w;
	// Read the moments from the variance shadow map.
	vec2 moments = texture(shadowMap, vec2(shadowCoord)).xy;

	// Compute the Chebyshev upper bound.
	return chebyshevUpperBound(moments, shadowCoord.z);
}


float evaluateShadows(Luz luz, vec3 pos){
	float sombreado = 1.0f;
	if( luz.shadowIndex == 0){
		vec4 shadowCoord = shadowMatrix0 * vec4(pos, 1.0f);	
		return shadowContribution(shadowMap0, shadowCoord);		
	}
	else if( luz.shadowIndex == 1){
		vec4 shadowCoord = shadowMatrix1 * vec4(pos, 1.0f);	
		return shadowContribution(shadowMap1, shadowCoord);			
	}
	else if( luz.shadowIndex == 2){
		vec4 shadowCoord = shadowMatrix2 * vec4(pos, 1.0f);	
		return shadowContribution(shadowMap2, shadowCoord);			
	}	
	return sombreado;
}

//*******************************************************************
//	Funciones del shader particular
//*******************************************************************

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

//*******************************************************************
//	Main
//*******************************************************************

void main()
{	
	//De aqui en mas se toman las coordenadas del parallax
	vec2 FixedTexCoord = ParallaxTexCoord(uvScale * (TexCoord + uvTranslation));
	
	// Proceso normal map y color difuso con las coordenadas de textura del parallax
	vec3 normal = normalize(vec3(texture( normalMap, FixedTexCoord )-vec4(0.5f, 0.5f, 0.5f, 0.0f)) * toObjectLocal) * normalFactor;	
	vec4 diffColor = texture(diffuseMap, FixedTexCoord);	
	Material material = mat;
	material.Kb = 100.0f;
	
	// Proceso la iluminacion
	vec3 iluminacion = vec3(0.0f);	
	float sombreado;
	for(int i=0; i < cantLucesActivas; i++){
		//Proceso el sombreado de la luz
		sombreado = evaluateShadows(luz[i], posicion);
		
		//Identifico el tipo de luz y la proceso				
		if(luz[i].posicion.w == 3.0f){			
			iluminacion += phongPuntualSoloDifusa(normal, luz[i], posicion, material, sombreado);
		}
		else if(luz[i].posicion.w == 0.0f){
			iluminacion += phongPuntual(normal, luz[i], posicion, material, sombreado);
		}
		else if(luz[i].posicion.w == 1.0f){
			iluminacion += phongDireccional(normal, luz[i], posicion, material, sombreado);
		}
		else{
			iluminacion += phongSpot(normal, luz[i], posicion, material, sombreado);
		}	
	}		
	vec3 finalColor = vec3(diffColor) * (material.Ka + iluminacion);
	//Aplico correcion gamma y salgo
	FragmentData = vec4(pow(finalColor, vec3(1.0f/gamma)) , diffColor.w );
}