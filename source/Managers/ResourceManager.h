#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture.h"
#include "Renderer/Sprite.h"

#define resources ResourceManager::get()

using namespace RenderEngine;

using std::map;
using std::pair;
using std::vector;
using std::string;
using glm::vec2;

using std::cout;
using std::endl;

class ResourceManager final
{
private:
	map<string, ShaderProgram*> shaderProgramsMap;
	map<string, Texture*> texturesMap;
	map<string, Sprite*> spritesMap;
	string directoryPath;
	ResourceManager() {}
	~ResourceManager() {}
	
	string getFileString(string fileName)
	{
		//std::ifstream fin("../Resources/Shaders/" + fileName + ".txt", std::ios::in);
		std::ifstream fin(directoryPath + fileName, std::ios::in);
		
		cout << directoryPath + fileName << endl;
		if (!fin.is_open())
			return string("");

		std::stringstream buffer;
		buffer << fin.rdbuf();
		return buffer.str();
	}

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator = (const ResourceManager&) = delete;
	
	void setExecutablePath(const string& executablePath)
	{
		directoryPath = executablePath;
		for (int i = 0; i < 3; ++i)
		{
			size_t found = directoryPath.find_last_of("/\\");
			directoryPath = directoryPath.substr(0, found);
			cout << directoryPath << endl;
		}
		directoryPath += "/";
	}


	void deleteAllResources()
	{
		for (auto it = shaderProgramsMap.cbegin(); it != shaderProgramsMap.cend(); ++it)
			delete it->second;
		shaderProgramsMap.clear();
		
		for (auto it = texturesMap.cbegin(); it != texturesMap.cend(); ++it)
			delete it->second;
		texturesMap.clear();

		for (auto it = spritesMap.cbegin(); it != spritesMap.cend(); ++it)
			delete it->second;
		spritesMap.clear();
	}
	
	static ResourceManager& get()
	{
		static ResourceManager object;
		return object;
	}

	ShaderProgram* loadShader(
		const string& programName,
		const string& vertexPath,
		const string& fragmentPath)
	{
		string vertexString = getFileString(vertexPath);
		if (vertexString.empty())
		{
			cout << "No vertexShader in loadShader(" << programName << ")" << endl;
			exit(-1);
		}

		string fragmentString = getFileString(fragmentPath);
		if (fragmentString.empty())
		{
			cout << "No fragmentShader in loadShader(" << programName << ")" << endl;
			exit(-1);
		}

		ShaderProgram* program = new ShaderProgram(vertexString, fragmentString);
		if (!program->isCompiled())
		{
			cout << "Shader is not compiled in loadShader(" << programName << ")" << endl;
			exit(-1);
		}

		shaderProgramsMap.emplace(programName, program);
		return program;
	}

	ShaderProgram* getShader(const string& programName)
	{
		auto it = shaderProgramsMap.find(programName);
		if (it == shaderProgramsMap.end())
		{
			cout << "Shader not found in getShader(" << programName << ")" << endl;
			exit(-1);
		}
		return it->second;
	}

	Texture* loadTexture(
		const string& textureName,
		const string& texturePath,
		const GLenum wrapMode = GL_CLAMP_TO_EDGE)
	{
		int channels = 0;
		int width = 0;
		int height = 0;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load(string(directoryPath + texturePath).c_str(), &width, &height, &channels, 0);

		if (!pixels)
		{
			cout << "Can not load texture in loadTexture(" << textureName << ")" << endl;
			exit(-1);
		}

		Texture* texture = new Texture(width, height, pixels, channels, GL_NEAREST, wrapMode);

		texturesMap.emplace(textureName, texture);
		stbi_image_free(pixels);
		return texture;
	}

	Texture* loadTextureAtlas(
		const string& textureName,
		const string& texturePath,
		const vector<pair<string, pair<vec2, vec2>>>& subTextureNames)
	{
		auto texture = loadTexture(textureName, texturePath);
		
		for (auto currentSubTextureName = subTextureNames.cbegin(); currentSubTextureName != subTextureNames.cend(); ++currentSubTextureName)
		{
			currentSubTextureName->second.second;
			
			glm::vec2 leftDownPoint = currentSubTextureName->second.first;
			glm::vec2 rightTopPoint = currentSubTextureName->second.second;

			glm::uvec2 size = vec2(rightTopPoint.x - leftDownPoint.x, rightTopPoint.y - leftDownPoint.y);

			leftDownPoint.x /= texture->width();
			leftDownPoint.y /= texture->height();

			rightTopPoint.x /= texture->width();
			rightTopPoint.y /= texture->height();

			texture->addSubTexture(currentSubTextureName->first, leftDownPoint, rightTopPoint, size);
		}
		return texture;
	}

	Texture* getTexture(const string& textureName)
	{
		auto it = texturesMap.find(textureName);
		if (it == texturesMap.end())
		{
			cout << "Texture not found in getTexture(" << textureName << ")" << endl;
			exit(-1);
		}
		return it->second;
	}
	
	Sprite* loadSprite(
		const string& spriteName,
		const string& textureName,
		const string& subTextureName,
		const string& shaderName)
	{
		auto texture = getTexture(textureName);
		auto shader = getShader(shaderName);

		Sprite* sprite = new Sprite(shader, texture, subTextureName);
		spritesMap.emplace(spriteName, sprite);
		
		return sprite;
	}

	Sprite* getSprite(const string& spriteName)
	{
		auto it = spritesMap.find(spriteName);
		if (it == spritesMap.end())
		{
			cout << "Sprite not found in getSprite(" << spriteName << ")" << endl;
			return nullptr;
		}
		return it->second;
	}

	void ResourceManager::loadJSONResources(const std::string& JSONPath)
	{
		const string JSONString = getFileString(JSONPath);
		if (JSONString.empty())
		{
			cout << "No JSON resources file!" << endl;
			exit(-1);
		}

		rapidjson::Document document;
		rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
		
		if (!parseResult)
		{
			cout << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << endl;
			cout << "In JSON file: " << JSONPath << endl;
			exit(-1);
		}

		auto shadersIt = document.FindMember("shaders");
		if (shadersIt != document.MemberEnd())
		{
			for (const auto& currentShader : shadersIt->value.GetArray())
			{
				const string name = currentShader["name"].GetString();
				const string vertexPath = currentShader["vertexPath"].GetString();
				const string fragmentPath = currentShader["fragmentPath"].GetString();
				loadShader(name, vertexPath, fragmentPath);
			}
		}

		auto textureAtlasesIt = document.FindMember("textureAtlases");
		if (textureAtlasesIt != document.MemberEnd())
		{
			for (const auto& currentTextrueAtlas : textureAtlasesIt->value.GetArray())
			{
				const std::string name = currentTextrueAtlas["name"].GetString();
				const std::string path = currentTextrueAtlas["path"].GetString();
				glm::vec2 leftDownPoint;
				glm::vec2 rightTopPoint;


				std::vector<std::pair<std::string, std::pair<glm::vec2, glm::vec2>>> subTextures;
				const auto subTexturesArray = currentTextrueAtlas["subTextures"].GetArray();

				subTextures.reserve(subTexturesArray.Size());
				
				auto subTexturesIt = currentTextrueAtlas.FindMember("subTextures");

				for (const auto& currentSubTexture : subTexturesIt->value.GetArray())
				{
					subTextures.emplace_back(std::make_pair(currentSubTexture["name"].GetString(),
							std::make_pair(vec2(currentSubTexture["leftDownPointX"].GetInt(),
								currentSubTexture["leftDownPointY"].GetInt()),
								vec2(currentSubTexture["rightTopPointX"].GetInt(),
									currentSubTexture["rightTopPointY"].GetInt()))));
				}
				loadTextureAtlas(name, path, subTextures);
			}
		}

		auto spritesIt = document.FindMember("sprites");
		if (spritesIt != document.MemberEnd())
		{
			for (const auto& currentSprite : spritesIt->value.GetArray())
			{
				const string name = currentSprite["name"].GetString();
				const string textureAtlas = currentSprite["textureAtlas"].GetString();
				const string shader = currentSprite["shader"].GetString();
				const string subTexture = currentSprite["initialSubTexture"].GetString();

				auto pSprite = loadSprite(name, textureAtlas, subTexture, shader);
				if (!pSprite)
				{
					continue;
				}

				auto framesIt = currentSprite.FindMember("frames");
				if (framesIt != currentSprite.MemberEnd())
				{
					const auto framesArray = framesIt->value.GetArray();
					vector<Sprite::FrameDescription> framesDescriptions;
					framesDescriptions.reserve(framesArray.Size());
					for (const auto& currentFrame : framesArray)
					{
						int offsetY = 0, offsetX = 0;
						if (currentFrame.HasMember("offsetY"))
							offsetY = currentFrame["offsetY"].GetInt();
						if (currentFrame.HasMember("offsetX"))
							offsetY = currentFrame["offsetX"].GetInt();

						
						
						const vec2 offset = vec2(offsetX, offsetY);
						const string subTextureStr = currentFrame["subTexture"].GetString();
						const double duration = currentFrame["duration"].GetDouble();
						const auto pTextureAtlas = getTexture(textureAtlas);
						const auto pSubTexture = pTextureAtlas->getSubTexture(subTextureStr);

						/*cout << "___________" << endl;
						cout << offsetX << "  " << offset.x << endl;
						cout << offsetY << "  " << offset.y << endl;*/

						framesDescriptions.emplace_back(pSubTexture.leftDownPoint, pSubTexture.rightTopPoint, offset, pSubTexture.size, duration);
					}
					pSprite->insertFrames(framesDescriptions);
				}
			}
		}
	}
};