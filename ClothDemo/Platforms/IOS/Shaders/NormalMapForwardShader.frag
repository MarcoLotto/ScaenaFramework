
precision mediump float;

varying mediump vec3 position;
varying mediump vec2 TexCoord;
varying mediump mat3 toObjectLocal;

//*******************************************************************
//	Variables del shader particular
//*******************************************************************

uniform sampler2D diffuseMap; // Los mapas a aplicar
uniform sampler2D normalMap; 

uniform vec2 uvTranslation;  // Transformaciones de la coordenadas UV
uniform vec2 uvScale;

uniform float normalFactor;  //Factor de atenuacion del NormalMap
uniform float decayFactor;  //Factor de decaimiento

 struct Material{
	mediump float Ka;
	mediump float Kd;
	mediump float Ks;
	mediump float Kb;
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
	float b = 0.05;
	float c = 1.0;
	float decaimiento = (luz.intensidad.x+luz.intensidad.y+luz.intensidad.z)/3.0 * (1.0/(b * float(length(luz.posicion.xyz - posicion)) + c));
			
	//vec3 v = normalize(vec3(-posicion));
	//vec3 r = normalize( v + LightDir );
	//return luz[i].intensidad*(mat.Ka + decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0f))) + mat.Ks*pow(decaimiento, 1.4f)*(pow( max(dot(r,norm), 0.0 ), mat.Kb)));	
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
//	Main
//*******************************************************************

void main()
{	
	vec2 finalTextureCoord = uvScale * (TexCoord + uvTranslation);
	vec3 normal = vec3(texture2D( normalMap, finalTextureCoord )-vec4(0.5, 0.5, 0.5, 0.0));
	normal.x *= decayFactor;
	normal.y *= decayFactor;	
	normal = normalFactor * normalize(normal * toObjectLocal);
	vec4 diffColor = texture2D(diffuseMap, finalTextureCoord);
	Material material = mat;
	material.Kb = 100.0;
	
	// Proceso la iluminacion
	vec3 iluminacion = vec3(0.0);
	float sombreado = 1.0;
	for(int i=0; i < cantLucesActivas; i++){
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
	vec3 finalColor = vec3(diffColor) * (material.Ka + iluminacion);
	//Aplico correcion gamma y salgo
	gl_FragColor = vec4(pow(finalColor, vec3(1.0/gamma)) , diffColor.w );
}