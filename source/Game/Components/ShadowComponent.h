#include "RenderComponent.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "../GameObject.h"
#include "../../Resources/ResourceManager.h"

class ShadowComponent : public RenderComponent
{
public:
	glm::vec2 localPosition = glm::vec2(0, 0);

	ShadowComponent() = delete;
	ShadowComponent(const ShadowComponent&) = delete;
	ShadowComponent& operator = (const ShadowComponent&) = delete;

	ShadowComponent(GameObject* gameObject) : RenderComponent(gameObject) {}
	~ShadowComponent() {}

	void render() override
	{
		if (!renderEnable)
			return;

		auto RC = gameObject->getComponent<SpriteRendererComponent>();
		if (!RC->renderEnable)
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

		position += RC->currentAnimation->localPosition + localPosition;

		bool isFlipped = false;

		RC->sprite->render(ResourceManager::getShader("shadowShader"),
			position,
			RC->currentAnimation->size,
			RC->currentAnimation->rotationPoint,
			rotation,
			layer,
			&(RC->currentAnimation->frames[RC->currentFrame]),
			isFlipped);
	}

	void setParametrs(const rapidjson::Value& jsonObject)
	{
		renderEnable = jsonObject["renderEnable"].GetBool();
		layer = jsonObject["layer"].GetInt();
		localPosition.x = jsonObject["localPositionX"].GetFloat();
		localPosition.y = jsonObject["localPositionY"].GetFloat();
	}
};
