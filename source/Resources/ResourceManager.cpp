#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "../../external/stb_image.h"

#include "ResourceManager.h"
#include "../System/Parser.h"



ShaderProgram* ResourceManager::loadShader(const std::string& programName,
										   const std::string& vertexPath,
										   const std::string& fragmentPath)
{
	std::string vertexString = Parser::getFileString(vertexPath);
	
	if (vertexString.empty())
	{
		std::cout << "Error! File with vertex shader is empty. File path: " << vertexPath << "." << std::endl;
		exit(-1);
	}

	std::string fragmentString = Parser::getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cout << "Error! File with fragment shader is empty. File path: " << vertexPath << "." << std::endl;
		exit(-1);
	}

	ShaderProgram* program = new ShaderProgram(vertexString, fragmentString);
	
	if (!program->isCompiled())
	{
		std::cout << "Error! Shader is not compiled. Shader name: " << programName << "." << std::endl;
		exit(-1);
	}

	shaders.emplace(programName, program);
	return program;
}

ShaderProgram* ResourceManager::getShader(const std::string& name)
{
	auto it = shaders.find(name);
	if (it == shaders.end())
	{
		std::cout << "Error! Shader not found. Shader name: " << name << "." << std::endl;
		exit(-1);
	}
	return it->second;
}

Texture* ResourceManager::loadTexture(const std::string& textureName,
									  const std::string& texturePath,
									  const std::vector<std::pair<std::string, std::pair<glm::vec2, glm::vec2>>>& subTextureNames)
{
	int channels = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(Parser::getDirectoryPath() + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels)
	{
		std::cout << "Error! Texture texture was not loaded. Texture name: " << textureName << "." << std::endl;
		exit(-1);
	}

	Texture* texture = new Texture(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE);
	textures[textureName] = texture;

	stbi_image_free(pixels);

	if (subTextureNames.empty())
	{
		std::cout << "Error! Texture does not contain subtextures. Texture name: " << textureName << "." << std::endl;
		exit(-1);
	}

	for (auto currentSubTextureName = subTextureNames.cbegin(); currentSubTextureName != subTextureNames.cend(); ++currentSubTextureName)
	{
		currentSubTextureName->second.second;

		glm::uvec2 size = currentSubTextureName->second.second;

		glm::vec2 leftDownPoint = currentSubTextureName->second.first;
		glm::vec2 rightTopPoint = glm::vec2(leftDownPoint.x + size.x, leftDownPoint.y + size.y);

		leftDownPoint.x /= texture->width() + 0.01f;
		leftDownPoint.y /= texture->height() + 0.01f;

		rightTopPoint.x /= texture->width() - 0.01f;
		rightTopPoint.y /= texture->height() - 0.01f;

		texture->addSubTexture(currentSubTextureName->first, leftDownPoint, rightTopPoint, size);
	}
	

	return texture;
}

Texture* ResourceManager::getTexture(const std::string& name)
{
	auto it = textures.find(name);
	if (it == textures.end())
	{
		std::cout << "Error! Texture not found. Texture name: " << name << "." << std::endl;
		exit(-1);
	}
	return it->second;
}

Sprite* ResourceManager::loadSprite(const std::string& spriteName, const std::string& textureName)
{
	Sprite* sprite = new Sprite(getTexture(textureName));
	sprites.emplace(std::make_pair(spriteName, sprite));
	return sprite;
}

Sprite* ResourceManager::getSprite(const std::string& name)
{
	auto it = sprites.find(name);
	if (it == sprites.end())
	{
		std::cout << "Error! Texture not found. Texture name: " << name << "." << std::endl;
		exit(-1);
	}
	return it->second;
}

void ResourceManager::loadResources(const std::string& filePath)
{
	rapidjson::Document document;
	Parser::parseJsonFile(filePath, &document);

	auto shadersIt = document.FindMember("Shaders");
	if (shadersIt != document.MemberEnd())
	{
		for (const auto& currentShader : shadersIt->value.GetArray())
		{
			const std::string name = currentShader["name"].GetString();
			const std::string vertexPath = currentShader["vertexPath"].GetString();
			const std::string fragmentPath = currentShader["fragmentPath"].GetString();
			loadShader(name, vertexPath, fragmentPath);
		}
	}

	auto texturesIt = document.FindMember("Textures");
	if (texturesIt != document.MemberEnd())
	{
		for (const auto& currentTexture : texturesIt->value.GetArray())
		{
			const std::string name = currentTexture["name"].GetString();
			const std::string path = currentTexture["path"].GetString();
			std::vector<std::pair<std::string, std::pair<glm::vec2, glm::vec2>>> subTextures;
			

			subTextures.reserve(currentTexture["subTextures"].GetArray().Size());

			auto subTexturesIt = currentTexture.FindMember("subTextures");

			for (const auto& currentSubTexture : subTexturesIt->value.GetArray())
			{
				subTextures.emplace_back(
					std::make_pair(currentSubTexture["name"].GetString(),
						std::make_pair(glm::vec2(currentSubTexture["x"].GetInt(), currentSubTexture["y"].GetInt()),
							glm::vec2(currentSubTexture["w"].GetInt(), currentSubTexture["h"].GetInt()))));
			}
			loadTexture(name, path, subTextures);
		}
	}

	auto spritesIt = document.FindMember("Sprites");
	if (spritesIt != document.MemberEnd())
	{
		for (const auto& currentSprite : spritesIt->value.GetArray())
		{
			const std::string name = currentSprite["name"].GetString();
			const std::string textureName = currentSprite["texture"].GetString();
			loadSprite(name, textureName);
		}
	}
}

void ResourceManager::deleteAllResources()
{
	for (auto it = shaders.cbegin(); it != shaders.cend(); ++it)
		delete it->second;
	shaders.clear();

	for (auto it = textures.cbegin(); it != textures.cend(); ++it)
		delete it->second;
	textures.clear();
}

std::map<std::string, ShaderProgram*> ResourceManager::shaders;
std::map<std::string, Texture*> ResourceManager::textures;
std::map<std::string, Sprite*> ResourceManager::sprites;