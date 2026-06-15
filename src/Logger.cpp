#include "Logger.h"
#include "properties.h"

#include <iostream>


#include <cerrno>  // Añadir esto
#include <cstring> // Añadir esto

fs::path Logger::_log_dir = "";
fs::path Logger::_main_dir = fs::current_path();
std::ofstream Logger::_file;

void Logger::Init() {
#ifdef LOG
	//Nombre de log
	time_t time_lcl = time(NULL);
	tm datetime;

	#ifdef _WIN32
    	localtime_s(&datetime, &time_lcl);
	#else
	    localtime_r(&time_lcl, &datetime); // Nota el cambio de orden
	#endif

	char log_name[24];
	strftime(log_name, 24, "log_%G%m%d_%H%M%S.txt", &datetime);
	std::cout << "Log file: " << log_name << std::endl;

	//Obtencion de la ruta
	_log_dir = _main_dir / LOG_PATH / log_name;
	// _log_dir += _main_dir.generic_string();
	// _log_dir += LOG_PATH;
	// _log_dir += log_name;

	_file.open(_log_dir, std::ios::app);



	if (!_file.is_open()) {
        std::cerr << "--- ERROR DE LOG ---" << std::endl;
        std::cerr << "Ruta intentada: " << _log_dir << std::endl;
        std::cerr << "Error del sistema: " << std::strerror(errno) << std::endl;
    
		exit(EXIT_FAILURE);
    }
#endif //LOG
}
void Logger::End() {
#ifdef LOG
	if (_file.is_open())
		_file.close();
	else
		std::cerr << "[ERROR] No se ha cerrado el archivo correctamente";
#endif //LOG
}
void Logger::Log(const std::stringstream& info){
#ifdef LOG

	/*_file.open(_log_dir.data(), std::ios::app);*/
	if (!_file.is_open()) 
		std::cerr << "[ERROR] No se ha abierto el archivo correctamente";

	_file << info.str();
	_file.flush();

#endif //LOG
}
void Logger::Log(const std::string& info){
#ifdef LOG
	if (!_file.is_open())
		std::cerr << "[ERROR] No se ha abierto el archivo correctamente";

	_file << info;
	_file.flush();

#endif //LOG
}
void Logger::Viz(const std::string& info) {
#ifdef DEBUG
	std::cout << info;
#endif // DEBUG
}
void Logger::Viz(const std::stringstream& info) {
#ifdef DEBUG
	std::cout << info.str();
#endif // DEBUG
}

