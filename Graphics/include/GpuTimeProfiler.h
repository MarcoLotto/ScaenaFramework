/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

// Permite obtener tiempos de la GPU
class GpuTimeProfiler{
	
public:	
	// Genera un nuevo profile y devuelve su identificador (genera memoria no usar todo el tiempo)
	static unsigned int reservProfile();

	// Dado un handler, empieza el profile de la GPU. Se medira cuando tiempo pasa entre esta llamada y la llamada a endProfile (tiempo GPU)
	static void startProfile(unsigned int profileHandler);

	// Marca en donde termina el profile de la GPU con este handler
	static void endProfile();

	// Solicita el tiempo de GPU registrado para el handler. Tener cuidado porque si no esta listo el resultado se produce un stall de CPU hasta que este listo.
	static unsigned int requestProfileResult(unsigned int profileHandler);

	// Borra de la memoria el profiler
	static void deleteProfile(unsigned int profileHandler);
};