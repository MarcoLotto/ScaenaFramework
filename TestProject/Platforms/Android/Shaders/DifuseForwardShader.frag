
precision mediump float;
precision mediump int;

varying mediump vec3 position;
varying mediump vec3 normal;
varying mediump vec2 TexCoord;

//*******************************************************************
//	Variables del shader particular
//*******************************************************************

uniform sampler2D diffuseMap; // El mapa difuso a aplicar

uniform highp vec2 uvTranslation;  // Transformaciones de la coordenadas UV
uniform highp vec2 uvScale;

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

uniform Luz luz[10];
uniform int cantLucesActivas;

uniform float gamma;  //Correcion gamma

//Parametros de las sombras, usar el mismo indice para ambos!
uniform mediump mat4 shadowMatrix0;  
uniform sampler2D shadowMap0;
uniform mediump mat4 shadowMatrix1;  
uniform sampler2D shadowMap1;
uniform mediump mat4 shadowMatrix2;  
uniform sampler2D shadowMap2;


//*******************************************************************
//	Funciones de iluminacion
//*******************************************************************

vec3 phongPuntual(vec3 norm, Luz luz, vec3 posicion, Material mat, float sombreado){
		
	vec3 LightDir = normalize( luz.posicion.xyz - posicion );
			
	//Calculo el factor de decaimiento
	float b = 0.05;
	float c = 1.0;
	float decaimiento = (luz.intensidad.x+luz.intensidad.y+luz.intensidad.z)/3.0 * (1.0/(b * float(length(luz.posicion.xyz - posicion)) + c));
			
	//vec3 v = normalize(vec3(-posicion));
	//vec3 r = normalize( v + LightDir );
	//return luz[i].intensidad*(mat.Ka + decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0))) + mat.Ks*pow(decaimiento, 1.4)*(pow( max(dot(r,norm), 0.0 ), mat.Kb)));	
	vec3 r = reflect( -LightDir, norm );
	vec3 v = normalize(vec3(-posicion));		
	return luz.intensidad*(sombreado*decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0))) + sombreado*mat.Ks*pow(decaimiento, 1.4)*(pow( max(dot(r,v), 0.0 ), mat.Kb)));
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
	return luz.intensidad*(sombreado*(mat.Kd*(max(dot(LightDir,norm), 0.0)) + mat.Ks*(pow( max(dot(r,v), 0.0 ), mat.Kb))));
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
	return finalIntensity*(sombreado*decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0))) + sombreado*sombreado*mat.Ks*pow(decaimiento, 1.4)*(pow( max(dot(r,v), 0.0 ), mat.Kb)));
}	

//*******************************************************************
//	Funciones de sombras
//*******************************************************************

float ReduceLightBleeding(float p_max, float amount)
{
	return clamp((p_max - amount) / (1.0 - amount), 0.0, 1.0);
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
	vec2 moments = texture2D(shadowMap, vec2(shadowCoord)).xy;

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


//*******************************************************************
//	Main
//*******************************************************************

void main()
{	
// Consigo el color difuso del fragmento
	vec3 diffColor = vec3(texture2D(diffuseMap, uvScale * (TexCoord + uvTranslation)));
	Material material = mat;
	material.Kb = 100.0;
	
	// Proceso la iluminacion
	vec3 iluminacion = vec3(0.0);	
	float sombreado = 1.0;
	for(int i=0; i < cantLucesActivas; i++){
		//Proceso el sombreado de la luz
		sombreado = evaluateShadows(luz[i], position);

		//Identifico el tipo de luz y la proceso				
		if(luz[i].posicion.w == 3.0){			
			iluminacion += phongPuntualSoloDifusa(normal, luz[i], position, material, sombreado);
		}
		else if(luz[i].posicion.w == 0.0){
			iluminacion += phongPuntual(normal, luz[i], position, material, sombreado);
		}
		else if(luz[i].posicion.w == 1.0){
			iluminacion += phongDireccional(normal, luz[i], position, material, sombreado);
		}
		else{
			iluminacion += phongSpot(normal, luz[i], position, material, sombreado);
		}	
	}		
	vec3 finalColor = diffColor * (material.Ka + iluminacion);
	//Aplico correcion gamma y salgo	
	gl_FragColor = vec4(pow(finalColor, vec3(1.0/gamma)) , 1.0 );
}
