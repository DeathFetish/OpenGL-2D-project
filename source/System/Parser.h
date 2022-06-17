#pragma once
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

class Parser
{
private:
	static std::string directoryPath;

public:
	static void setDirectoryPath(const std::string& directoryPath);
	static const std::string getDirectoryPath();
	static const std::string getFileString(const std::string& filePath);
	static void parseJsonFile(const std::string& filePath, rapidjson::Document* outDocument);
};