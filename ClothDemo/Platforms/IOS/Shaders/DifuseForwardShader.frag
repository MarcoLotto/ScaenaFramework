
precision mediump float;

varying highp vec3 position;
varying highp vec3 normal;
varying highp vec2 TexCoord;

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
	mediump vec4 posicion;
	mediump vec3 intensidad;
	mediump vec3 direccion;
	mediump float spotFactor;

	mediump int shadowIndex;  //El indice de shadowMap que le corresponde (-1 si ninguno)
};

uniform Luz luz[5];
uniform int cantLucesActivas;

uniform float gamma;  //Correcion gamma


//*******************************************************************
//	Funciones de iluminacion
//*******************************************************************

vec3 phongPuntual(vec3 norm, Luz luz, vec3 posicion, Material mat){
		
	vec3 LightDir = normalize( luz.posicion.xyz - posicion );
			
	//Calculo el factor de decaimiento
	float b = 0.05;
	float c = 1.0;
	float decaimiento = (luz.intensidad.x+luz.intensidad.y+luz.intensidad.z)/3.0 * (1.0/(b * float(length(luz.posicion.xyz - posicion)) + c));

    vec3 r = reflect( -LightDir, norm );
	vec3 v = normalize(vec3(-posicion));		
	return luz.intensidad*(decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0))) + mat.Ks*pow(decaimiento, 1.4)*(pow( max(dot(r,v), 0.0 ), mat.Kb)));
}	

vec3 phongPuntualSoloDifusa(vec3 norm, Luz luz, vec3 posicion, Material mat){
		
	vec3 LightDir = normalize( luz.posicion.xyz - posicion );
			
	//Calculo el factor de decaimiento
	float b = 0.05;
	float c = 1.0;
	float decaimiento = (luz.intensidad.x+luz.intensidad.y+luz.intensidad.z)/3.0 * (1.0/(b * float(length(luz.posicion.xyz - posicion)) + c));
	
	vec3 v = normalize(vec3(-posicion));		
	return luz.intensidad*(decaimiento*(mat.Kd*max(dot(LightDir,norm), 0.0)));
}	

vec3 phongDireccional(vec3 norm, Luz luz, vec3 posicion, Material mat){
		
	vec3 LightDir = -normalize( luz.direccion );
	
	vec3 r = reflect( -LightDir, norm );
	vec3 v = normalize(vec3(-posicion));		
	return luz.intensidad*((mat.Kd*(max(dot(LightDir,norm), 0.0)) + mat.Ks*(pow( max(dot(r,v), 0.0 ), mat.Kb))));
}	

vec3 phongSpot(vec3 norm, Luz luz, vec3 posicion, Material mat){
	
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
	return finalIntensity*(decaimiento*(mat.Kd*(max(dot(LightDir,norm), 0.0))) + mat.Ks*pow(decaimiento, 1.4)*(pow( max(dot(r,v), 0.0 ), mat.Kb)));
}	

//*******************************************************************
//	Main
//*******************************************************************

void main()
{	
// Consigo el color difuso del fragmento
	vec4 diffColor = texture2D(diffuseMap, uvScale * (TexCoord + uvTranslation));
	Material material = mat;
	material.Kb = 100.0;
	
	// Proceso la iluminacion
	vec3 iluminacion = vec3(0.0);
    iluminacion += float(cantLucesActivas >= 0) * phongSpot(normal, luz[0], position, material);
	//iluminacion += float(cantLucesActivas >= 1) * phongPuntualSoloDifusa(normal, luz[1], position, material);
    //iluminacion += float(cantLucesActivas >= 2) * phongPuntual(normal, luz[2], position, material);
    //iluminacion += float(cantLucesActivas >= 3) * phongDireccional(normal, luz[3], position, material);
	
	//Aplico correcion gamma y salgo
    vec3 finalColor = vec3(diffColor) * (material.Ka + iluminacion);
	gl_FragColor = vec4(pow(finalColor, vec3(1.0/gamma)) , diffColor.w );
}
