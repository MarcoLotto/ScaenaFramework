#version 300 es

precision mediump float;
precision mediump int;

in vec2 TexCoord;

out vec4 FragColor;

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

//Tengo que contar con la matriz de proyeccion de la escena para pasar de la textura de depth a la posicion real
uniform mat4 sceneProj;

//Parametros de las sombras, usar el mismo indice para ambos!
uniform mat4 shadowMatrix0;  
uniform sampler2D shadowMap0;
uniform mat4 shadowMatrix1;  
uniform sampler2D shadowMap1;
uniform mat4 shadowMatrix2;  
uniform sampler2D shadowMap2;

//Uniforms con los datos del deferred shading
uniform sampler2D DepthTex;
uniform sampler2D NormalTex;
uniform sampler2D ColorTex;
uniform sampler2D MaterialTex;

 struct Material{
	float Ka;
	float Kd;
	float Ks;
	float brillo;
};

vec3 phongPuntual(vec3 norm, Luz luz, vec3 posicion, Material mat, float sombreado){
		
	vec3 LightDir = normalize( luz.posicion.xyz - posicion );
			
	//Calculo el factor de decaimiento
	float b = 0.05;
	float c = 1.0;
	float decaimiento = (luz.intensidad.x+luz.intensidad.y+luz.intensidad.z)/3.0 * (1.0/(b * float(length(luz.posicion.xyz - posicion)) + c));
			
	//vec3 v = normalize(vec3(-posicion));
	//vec3 r = normalize( v + LightDir );
	//return luz[i].intensidad*(mat.Ka + decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0))) + mat.Ks*pow(decaimiento, 1.4)*(pow( max(dot(r,norm), 0.0 ), mat.brillo)));	
	vec3 r = reflect( -LightDir, norm );
	vec3 v = normalize(vec3(-posicion));		
	return luz.intensidad*(sombreado*decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0))) + sombreado*mat.Ks*pow(decaimiento, 1.4)*(pow( max(dot(r,v), 0.0 ), mat.brillo)));
}	

vec3 phongPuntualSoloDifusa(vec3 norm, Luz luz, vec3 posicion, Material mat, float sombreado){
		
	vec3 LightDir = normalize( luz.posicion.xyz - posicion );
			
	//Calculo el factor de decaimiento
	float b = 0.05;
	float c = 1.0;
	float decaimiento = (luz.intensidad.x+luz.intensidad.y+luz.intensidad.z)/3.0 * (1.0/(b * float(length(luz.posicion.xyz - posicion)) + c));			
	
	vec3 v = normalize(vec3(-posicion));		
	return luz.intensidad*(sombreado*decaimiento*(mat.Kd*max(dot(LightDir,norm), 0.0)));
}	

vec3 phongDireccional(vec3 norm, Luz luz, vec3 posicion, Material mat, float sombreado){
		
	vec3 LightDir = -normalize( luz.direccion );
	
	vec3 r = reflect( -LightDir, norm );
	vec3 v = normalize(vec3(-posicion));		
	return luz.intensidad*(sombreado*(mat.Kd*(max(dot(LightDir,norm), 0.0)) + mat.Ks*(pow( max(dot(r,v), 0.0 ), mat.brillo))));
}	

vec3 phongSpot(vec3 norm, Luz luz, vec3 posicion, Material mat, float sombreado){
	
	//Primero planteo como si fuese una luz puntual o spot
	vec3 LightDir = normalize( luz.posicion.xyz - posicion );

	//Calculo el decaimiento radial
	float decaimSpot = max(1.0 - dot(LightDir,  luz.direccion), 0.0);		
	decaimSpot = pow(decaimSpot, luz.spotFactor); 
	
	//Calculo el factor de decaimiento
	float b = 0.05;
	float c = 1.0;
	float decaimiento = (luz.intensidad.x+luz.intensidad.y+luz.intensidad.z)/3.0 * (1.0/(b * float(length(luz.posicion.xyz - posicion)) + c));
	decaimiento *= decaimSpot;

	// Calculo phong, dividiendo la intensidad por el factor de spot (sino se desproporciona la intensidad con factores muy grandes)
	vec3 r = reflect( -LightDir, norm );
	vec3 v = normalize(vec3(-posicion));
	vec3 finalIntensity = luz.intensidad / luz.spotFactor;
	return finalIntensity*(sombreado*decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0))) + sombreado*sombreado*mat.Ks*pow(decaimiento, 1.4)*(pow( max(dot(r,v), 0.0 ), mat.brillo)));
}	

float ReduceLightBleeding(float p_max, float amount)
{
	return (p_max - amount) / (1.0 - amount); 
}

float chebyshevUpperBound(vec2 moments, float t)
{
	float min_variance = 0.07;

	// One-tailed inequality valid if t > moments.x
    float p = 0.0;
	if(t <= moments.x){
		p = 1.0;
	}

	// Compute variance
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance, min_variance);

	// Compute probabilistic upper bound.
	float d = t - moments.x;
	float p_max = variance / (variance + d*d);

	//Se reduce el light bleeding, variar el segundo parametro para mayor o menor efecto
	//p_max = ReduceLightBleeding(p_max, 0.2);

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
	float sombreado = 1.0;
	if( luz.shadowIndex == 0){
		vec4 shadowCoord = shadowMatrix0 * vec4(pos, 1.0);	
		return shadowContribution(shadowMap0, shadowCoord);		
	}
	else if( luz.shadowIndex == 1){
		vec4 shadowCoord = shadowMatrix1 * vec4(pos, 1.0);	
		return shadowContribution(shadowMap1, shadowCoord);			
	}
	else if( luz.shadowIndex == 2){
		vec4 shadowCoord = shadowMatrix2 * vec4(pos, 1.0);	
		return shadowContribution(shadowMap2, shadowCoord);			
	}	
	return sombreado;
}

// A partir de la textura de depth se recontruye la posicion del fragmento en el subespacio de view-model
vec3 reconstructPositionFromDepth(){ 
    float depth = texture( DepthTex, TexCoord ).x * 2.0 - 1.0;
    float viewDepth = -sceneProj[3][2] / (depth + sceneProj[2][2]);
	vec2 nDc = (2.0 * TexCoord - vec2(1.0)) * viewDepth;
    vec3 viewPosition = vec3(nDc.x / -sceneProj[0][0], nDc.y / -sceneProj[1][1], viewDepth);
	return viewPosition;
}


// NOTA: Este es un shader generico, por lo que puede ser optimizado si se conoce el numero
// de luces en escena. De ser asi se puede eliminar el for dentro del main y se conseguira un boost de performance
void main()
{	
	vec3 pos = reconstructPositionFromDepth();
	vec3 norm = vec3( texture( NormalTex, TexCoord ) );
	vec3 diffColor = vec3( texture(ColorTex, TexCoord) );
	vec3 matTex = vec3(texture(MaterialTex, TexCoord) );
	Material mat;
	mat.Ka = matTex.x;
	mat.Kd = matTex.y;
	mat.Ks = matTex.z;
	mat.brillo = 100.0;
	
	vec3 iluminacion = vec3(0.0);
	
	float sombreado;
	for(int i=0; i < 4; i++){
		//Proceso el sombreado de la luz
		sombreado = evaluateShadows(luz[i], pos);
		
		//Identifico el tipo de luz y la proceso				
		if(luz[i].posicion.w == 3.0){			
			iluminacion += phongPuntualSoloDifusa(norm, luz[i], pos, mat, sombreado);
		}
		else if(luz[i].posicion.w == 0.0){
			iluminacion += phongPuntual(norm, luz[i], pos, mat, sombreado);
		}
		else if(luz[i].posicion.w == 1.0){
			iluminacion += phongDireccional(norm, luz[i], pos, mat, sombreado);
		}
		else{
			iluminacion += phongSpot(norm, luz[i], pos, mat, sombreado);
		}	
	}		
	vec3 finalColor = diffColor * (mat.Ka + iluminacion);
	//Aplico correcion gamma y salgo
	FragColor = vec4(pow(finalColor, vec3(1.0/gamma)) , 1.0 );
}