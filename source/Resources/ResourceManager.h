#pragma once
#include <string>
#include <vector>
#include <map>

#include <glm/vec2.hpp>

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Sprite.h"

class ResourceManager
{
private:
	static std::map<std::string, ShaderProgram*> shaders;
	static std::map<std::string, Texture*> textures;
	static std::map<std::string, Sprite*> sprites;

public:
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator = (const ResourceManager&) = delete;

	static ShaderProgram* loadShader(const std::string& name,
									 const std::string& vertexPath,
									 const std::string& fragmentPath);

	static ShaderProgram* getShader(const std::string& name);

	static Texture* loadTexture(const std::string& textureName,
								const std::string& texturePath,
								const std::vector<std::pair<std::string, std::pair<glm::vec2, glm::vec2>>>& subTextureNames);

	static Texture* getTexture(const std::string& name);
	
	static Sprite* loadSprite(const std::string& spriteName, const std::string& textureName);

	static Sprite* getSprite(const std::string& name);

	static void loadResources(const std::string& filePath);

	static void deleteAllResources();
};