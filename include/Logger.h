#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

class Logger{
public:
	static void Init();
	static void End();
	static void Log(const std::stringstream & info);
	static void Log(const std::string& info);
	static void Viz(const std::string & info);
	static void Viz(const std::stringstream& info);
	static fs::path _log_dir;
	static fs::path _main_dir;
	static std::ofstream _file;
};

