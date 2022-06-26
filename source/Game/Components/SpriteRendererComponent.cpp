#pragma once
#include "../GameObject.h"
#include "../ObjectsManager.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

void SpriteRendererComponent::render()
{
	if (!renderEnable)
		return;

	glm::vec2 position(0, 0);
	float rotation = 0;

	auto object = gameObject;
	do
	{
		auto TC = object->getComponent<TransformComponent>();
		position += TC->position;
		rotation += TC->rotation;
	} while (object = object->getParent());

	position += currentAnimation->localPosition;

	bool isFlipped = false;

	sprite->render(shader, position,
		currentAnimation->size,
		currentAnimation->rotationPoint,
		rotation, layer,
		&(currentAnimation->frames[currentFrame]),
		isFlipped);
}

void SpriteRendererComponent::update()
{
	if (!animationEnabled)
		return;

	currentAnimationTime += Time::getDeltaTime();
	while (currentAnimationTime >= currentAnimation->frames[currentFrame].duration)
	{
		currentAnimationTime -= currentAnimation->frames[currentFrame].duration;
		++currentFrame;
		if (currentFrame == currentAnimation->frames.size())
			currentFrame = 0;
	}
}

void SpriteRendererComponent::addAnimation(const std::string name, SpriteAnimation& animation) { animations.insert(std::make_pair(name, animation)); }
void SpriteRendererComponent::deleteAnimation(const std::string name) { animations.erase(name); }
void SpriteRendererComponent::setCurrentAnimation(const std::string& animationName, const unsigned int frameId)
{
	if (animations.find(animationName) == animations.end())
	{
		std::cout << "Error! Can not find animation. Animation name: " << animationName << std::endl;
		exit(-1);
	}
	currentAnimation = &animations[animationName];

	if (frameId >= currentAnimation->frames.size())
		currentFrame = frameId >= currentAnimation->frames.size() ? 0 : frameId;
}

void SpriteRendererComponent::setParametrs(const rapidjson::Value& jsonObject)
{
	renderEnable = jsonObject["renderEnable"].GetBool();
	animationEnabled = jsonObject["animationEnable"].GetBool();
	layer = jsonObject["layer"].GetInt();

	sprite = ResourceManager::getSprite(jsonObject["Sprite"].GetString());
	shader = ResourceManager::getShader(jsonObject["Shader"].GetString());

	auto animationsIt = jsonObject.FindMember("Animations");
	if (animationsIt != jsonObject.MemberEnd())
	{
		for (const auto& currentAnimation : animationsIt->value.GetArray())
		{
			std::string name = currentAnimation["name"].GetString();
			glm::vec2 size = glm::vec2(currentAnimation["sizeX"].GetFloat(), currentAnimation["sizeY"].GetFloat());
			glm::vec2 localPosition = glm::vec2(currentAnimation["localPositionX"].GetFloat(), currentAnimation["localPositionY"].GetFloat());
			glm::vec2 rotationPoint = glm::vec2(currentAnimation["rotationPointX"].GetFloat(), currentAnimation["rotationPointY"].GetFloat());
			std::vector<FrameDescription> frames;

			auto texture = sprite->getTexture();

			auto framesIt = currentAnimation.FindMember("Frames");
			for (const auto& currentFrame : framesIt->value.GetArray())
			{
				double duration = currentFrame["duration"].GetDouble();
				auto subTexture = texture->getSubTexture(currentFrame["subTexture"].GetString());
				frames.emplace_back(subTexture.leftDownPoint, subTexture.rightTopPoint, duration);
			}
			animations.insert(std::make_pair(name, SpriteAnimation(localPosition, rotationPoint, size, frames)));
		}
	}

	setCurrentAnimation(jsonObject["currentAnimation"].GetString());
}
