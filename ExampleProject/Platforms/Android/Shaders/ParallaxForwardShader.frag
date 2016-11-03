
precision mediump float;
precision mediump int;

varying mediump vec3 posicion;
varying mediump vec2 TexCoord;
varying mediump mat3 toObjectLocal;

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

uniform Luz luz[10];
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
//	Funciones del shader particular
//*******************************************************************

vec2 ParallaxTexCoord(vec2 textureCoord){
	float s = displacementFactor * atenuationFactor;
	float maxIt = 6.0;
	float minIt = 4.0;
	float farDist = 0.6;
	float h = 0.0;
	vec3 hP = vec3(0.0);
		
	//Encuentro el vector de vista en coordenadas locales	
	vec3 dir = normalize(toObjectLocal * posicion);
	vec3 n = vec3(0.0, 0.0, 1.0);

	//Hago un primer fetch
	vec3 v = vec3(textureCoord, 0.0) - (farDist)*dir * s;
	h= texture2D(displacementMap, vec2(v)).x - 0.5;
	vec3 hPAnt = vec3(v.x, v.y, s*h);
	
	//Encuentro muestras de altura de varios puntos y calculo un promedio
	maxIt = minIt + maxIt - maxIt*dot(-n, dir)/2.0; 
	for(int i = 1; float(i) <= maxIt ; i++){		
		float alpha = float(i)*farDist/maxIt;
		v = vec3(textureCoord, 0.0) - (farDist - alpha)*dir * s;
		h= texture2D(displacementMap, vec2(v)).x - 0.5;
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
	vec3 normal = normalize(vec3(texture2D( normalMap, FixedTexCoord )-vec4(0.5, 0.5, 0.5, 0.0)) * toObjectLocal) * normalFactor;	
	vec3 diffColor = vec3(texture2D(diffuseMap, FixedTexCoord));	
	Material material = mat;
	material.Kb = 100.0;
	
	// Proceso la iluminacion
	vec3 iluminacion = vec3(0.0);	
	float sombreado = 1.0;
	for(int i=0; i < cantLucesActivas; i++){
		//Identifico el tipo de luz y la proceso				
		if(luz[i].posicion.w == 3.0){			
			iluminacion += phongPuntualSoloDifusa(normal, luz[i], posicion, material, sombreado);
		}
		else if(luz[i].posicion.w == 0.0){
			iluminacion += phongPuntual(normal, luz[i], posicion, material, sombreado);
		}
		else if(luz[i].posicion.w == 1.0){
			iluminacion += phongDireccional(normal, luz[i], posicion, material, sombreado);
		}
		else{
			iluminacion += phongSpot(normal, luz[i], posicion, material, sombreado);
		}	
	}		
	vec3 finalColor = diffColor * (material.Ka + iluminacion);
	//Aplico correcion gamma y salgo
	gl_FragColor = vec4(pow(finalColor, vec3(1.0/gamma)) , 1.0 );
}