#include <fstream>

#include "utility/logger.hpp"

using namespace Hippocrates;
using namespace Hippocrates::Utility;

auto Logger::CreateLoggingDirs() -> void {
	timestamp = std::chrono::system_clock::now();

	auto currentDir = GetCurrentDir();
	auto logFolder = GetLogFolder();
	auto logFileExtension = GetLogFileExtension();


	auto dumpDir(currentDir + logFolder);
	if (!Type::Filesystem::exists(dumpDir)) {
		Type::Filesystem::create_directory(dumpDir);
	}


	auto sessionDir = GetSessionDir(dumpDir);
	if (!Type::Filesystem::exists(sessionDir)) {
		Type::Filesystem::create_directory(sessionDir);
	}

	SetFullLoggingPath(sessionDir);
}

auto Logger::GetCurrentDir() -> Type::file_string_t {
	return Type::Filesystem::current_path();
}

auto Logger::GetLogFolder() -> Type::file_string_t {
	return HIPPOCRATES_LITERAL_AS_FILE_STRING("/json_dumps/");
}

auto Logger::GetLogFileExtension() -> Type::file_string_t {
	return HIPPOCRATES_LITERAL_AS_FILE_STRING(".json");
}


auto Logger::GetSessionDir(const Type::file_string_t& dumpDir) -> Type::file_string_t {
	return Type::file_string_t(dumpDir + Type::to_file_string(timestamp.time_since_epoch().count()) + HIPPOCRATES_LITERAL_AS_FILE_STRING("/"));
}

auto Logger::GetMetadataFileName(const Type::file_string_t &sessionDir) -> Type::file_string_t {
	return sessionDir + HIPPOCRATES_LITERAL_AS_FILE_STRING("meta") + GetLogFileExtension();
}


auto Logger::GetLogFileName(const Type::file_string_t& sessionDir, std::size_t generationsPassed) -> Type::file_string_t {
	return Type::file_string_t(sessionDir + HIPPOCRATES_LITERAL_AS_FILE_STRING("generation_") + Type::to_file_string(generationsPassed) + GetLogFileExtension());
}


auto Logger::SetFullLoggingPath(const Type::file_string_t& path) -> void {
	fullLoggingPath = path;
}


auto Logger::LogGeneration(std::size_t generation, const std::string& log) -> void {
	if (fullLoggingPath.empty()) 
		throw std::runtime_error("No logging directory found. Did you forget to call Logger::CreateLoggingDirs()?");
	
	auto logFileName = GetLogFileName(fullLoggingPath, generation);
	std::ofstream logFile(logFileName);
	logFile << log;
	logFile.close();
}

auto Logger::LogMetadata(Type::fitness_t maxFitness) -> void {
	metaData += "{\"max_fitness\":" + std::to_string(maxFitness) + "},";
	auto log = "{\"generations\":[" + metaData.substr(0, metaData.length() - 1) + "]}";

	if (fullLoggingPath.empty()) 
		throw std::runtime_error("No logging directory found. Did you forget to call Logger::CreateLoggingDirs()?");
	
	auto logFileName = GetMetadataFileName(fullLoggingPath);
	std::ofstream logFile(logFileName);
	logFile << log;
	logFile.close();
}
