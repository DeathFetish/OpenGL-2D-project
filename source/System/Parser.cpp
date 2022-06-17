#include <iostream>
#include <fstream>
#include <sstream>

#include "Parser.h"

void Parser::setDirectoryPath(const std::string& path) 
{
	directoryPath = path;
	for (int i = 0; i < 3; ++i)
	{
		size_t found = directoryPath.find_last_of("/\\");
		directoryPath = directoryPath.substr(0, found);
	}
	directoryPath += "/";
}

const std::string Parser::getDirectoryPath() 
{ 
	return directoryPath; 
}

const std::string Parser::getFileString(const std::string& fileName)
{
	std::ifstream fin(directoryPath + fileName, std::ios::in);

	if (!fin.is_open())
		return std::string("");

	std::stringstream buffer;
	buffer << fin.rdbuf();
	return std::string(buffer.str());
}


void Parser::parseJsonFile(const std::string& filePath, rapidjson::Document* outDocument)
{
	const std::string JSONString = getFileString(filePath);

	if (JSONString.empty())
	{
		std::cout << "Error! No JSON objects in file. File path: " << filePath << "." << std::endl;
		exit(-1);
	}

	rapidjson::ParseResult parseResult = outDocument->Parse(JSONString.c_str());

	if (!parseResult)
	{
		std::cout << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl;
		std::cout << "File path: " << filePath << std::endl;
		exit(-1);
	}
}

std::string Parser::directoryPath;