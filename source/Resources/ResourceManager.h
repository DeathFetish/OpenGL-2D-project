#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#define resources ResourceManager::get()

using namespace std;
using namespace RenderEngine;

class ResourceManager final
{
private:
	map<string, ShaderProgram*> shaderProgramsMap;
	map<string, Texture*> texturesMap;
	map<string, Sprite*> spritesMap;
	map<string, AnimatedSprite*> animatedSpritesMap;
	
	ResourceManager() {}
	~ResourceManager() {}
	
	string getFileString(string fileName)
	{
		ifstream fin("../Resources/Shaders/" + fileName + ".txt", ios::in);
		if (!fin.is_open())
		{
			cout << fileName << " shader is not open" << endl;
			return string("");
		}
		stringstream buffer;
		buffer << fin.rdbuf();
		return buffer.str();
	}

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator = (const ResourceManager&) = delete;
	
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

		for (auto it = animatedSpritesMap.cbegin(); it != animatedSpritesMap.cend(); ++it)
			delete it->second;
		animatedSpritesMap.clear();
	}
	
	static ResourceManager& get()
	{
		static ResourceManager object;
		return object;
	}

	ShaderProgram* loadShader(const string& vertexPath, const string& fragmentPath, const string& programName)
	{
		string vertexString = getFileString(vertexPath);
		if (vertexString.empty())
			return nullptr;

		string fragmentString = getFileString(fragmentPath);
		if (fragmentString.empty())
			return nullptr;

		ShaderProgram* program = new ShaderProgram(vertexString, fragmentString);
		if (!program->isCompiled())
			return nullptr;

		shaderProgramsMap.emplace(programName, program);
		return program;
	}

	ShaderProgram* getShader(const string& programName)
	{
		auto it = shaderProgramsMap.find(programName);
		if (it == shaderProgramsMap.end())
		{
			cout << programName << " shader not found" << endl;
			return nullptr;
		}
		return it->second;
	}

	Texture* loadTexture(const string& textureName, const string& texturePath, GLenum wrapMode = GL_CLAMP_TO_EDGE)
	{
		int channels = 0;
		int width = 0;
		int height = 0;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load(string("../Resources/Textures/" + texturePath + ".png").c_str(), &width, &height, &channels, 0);

		if (!pixels)
		{
			cout << "can not load texture " << textureName << endl;
			return nullptr;
		}

		Texture* texture = new Texture(width, height, pixels, channels, GL_NEAREST, wrapMode);

		texturesMap.emplace(textureName, texture);
		stbi_image_free(pixels);
		return texture;
	}

	Texture* loadTextureAtlas(const string& textureName,
							  const string& texturePath,
							  vector<pair<string, pair<vec2, vec2>>> subTextureNames)
	{
		auto texture = loadTexture(textureName, texturePath);
		
		if (!texture) return texture;
		
		for (auto currentSubTextureName = subTextureNames.cbegin(); currentSubTextureName != subTextureNames.cend(); ++currentSubTextureName)
		{
			currentSubTextureName->second.second;
			
			auto leftDownPoint = currentSubTextureName->second.first;
			leftDownPoint.x /= texture->width();
			leftDownPoint.y /= texture->height();

			auto rightTopPoint = currentSubTextureName->second.second;
			rightTopPoint.x /= texture->width();
			rightTopPoint.y /= texture->height();

			texture->addSubTexture(currentSubTextureName->first, leftDownPoint, rightTopPoint);
		}
		return texture;
	}

	Texture* getTexture(const string& textureName)
	{
		auto it = texturesMap.find(textureName);
		if (it == texturesMap.end())
		{
			cout << textureName << " texture not found" << endl;
			return nullptr;
		}
		return it->second;
	}
	
	Sprite* loadSprite(const string& spriteName,
					   const string& textureName,
					   const string& subTextureName,
					   const string& shaderName,
					   const vec2& position,
					   const vec2& size,
					   const vec2& repeatsForWrapMode = vec2(1, 1), 
					   const float rotation = 0)
	{
		auto texture = getTexture(textureName);
		auto shader = getShader(shaderName);

		Sprite* sprite = new Sprite(shader, texture, subTextureName, position, size, repeatsForWrapMode, rotation);
		spritesMap.emplace(spriteName, sprite);
		
		return sprite;
	}

	Sprite* getSprite(const string& spriteName)
	{
		auto it = spritesMap.find(spriteName);
		if (it == spritesMap.end())
		{
			cout << spriteName << " sprite not found" << endl;
			return nullptr;
		}
		return it->second;
	}

	AnimatedSprite* loadAnimatedSprite(const string& spriteName,
									   const string& textureName,
									   const string& subTextureName,
									   const string& shaderName,
									   const vec2& position,
									   const vec2& size,
									   const vec2& repeatsForWrapMode = vec2(1, 1),
									   const float rotation = 0)
	{
		auto texture = getTexture(textureName);
		auto shader = getShader(shaderName);

		AnimatedSprite* sprite = new AnimatedSprite(shader, texture, subTextureName, position, size, repeatsForWrapMode, rotation);
		animatedSpritesMap.emplace(spriteName, sprite);

		return sprite;
	}

	AnimatedSprite* getAnimatedSprite(const string& animatedSpriteName)
	{
		auto it = animatedSpritesMap.find(animatedSpriteName);
		if (it == animatedSpritesMap.end())
		{
			cout << animatedSpriteName << " animated sprite not found" << endl;
			return nullptr;
		}
		return it->second;
	}
};