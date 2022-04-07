#pragma once
#include <vector>
#include <string>

#include "Components.h"
#include "../System/Time.h"
#include "../Renderer/Camera.h"

class IGameObject
{
protected: 
	std::string tag;

	bool needUpdate = true;
	bool needRender = true;

public:
	virtual void render() = 0;
	virtual void update() = 0;
	
	virtual PhysicComponent* const getPhysicComponent() = 0;
	virtual RenderComponent* const getRenderComponent() = 0;
	virtual ShadowComponent* const getShadowComponent() = 0;
	virtual MovableComponent* const	getMovableComponent() = 0;
	virtual TransformComponent* const getTransformComponent() = 0;

	const string& getTag() const { return tag; }

	IGameObject() : tag("") {}
	IGameObject(const std::string& tag) : tag(tag) {}
	IGameObject(const IGameObject&) = delete;
	IGameObject& operator = (const IGameObject&) = delete;

	void enableUpdate(bool enable) { needUpdate = enable; }
	void enableRender(bool enable) { needRender = enable; }
};

class MapObject : public IGameObject
{
private:


	std::function<void(IGameObject* obj)> updateFunction;
	std::function<void(IGameObject* obj)> renderFunction;

	TransformComponent* const TC;
	RenderComponent* const RC;
	PhysicComponent* const PC;
	MovableComponent* const MC;
	ShadowComponent* const SC;

	static std::map<std::string, std::function<void(IGameObject*, IGameObject*, const glm::vec2&)>> walkHitBoxOnCollisionMap;
	static std::map<std::string, std::function<void(IGameObject*)>> updateFunctionsMap;
	static std::map<std::string, std::function<void(IGameObject*)>> renderFunctionsMap;

public:

	static void initMaps()
	{
		walkHitBoxOnCollisionMap.emplace("default", [](IGameObject* obj, IGameObject* obj2, const glm::vec2& direction) {});

		walkHitBoxOnCollisionMap.emplace("Door", [](IGameObject* obj, IGameObject* obj2, const glm::vec2& direction)
			{
				std::cout << "DoorOnCollision" << std::endl;

				auto TC = obj->getTransformComponent();
				auto PC = obj->getPhysicComponent();
				auto MC = obj->getMovableComponent();

				float sign = direction.y > 0 ? 1.f : -1.f;

				std::cout << TC->rotation << std::endl;

				MC->speed = 0.3f;

				if (direction.y > 0)
				{
					TC->rotation += 20.f;
				}
				else
				{
					//	TC->rotation = 360.f - TC->rotation + 20.f;
				}
				std::cout << TC->rotation << std::endl;

				PC->walkHitBox->recalculateIShape(TC->position, TC->rotation);
			});

		walkHitBoxOnCollisionMap.emplace("Bat", [](IGameObject* obj, IGameObject* obj2, const glm::vec2& direction) 
			{
				obj->enableRender(false);
				obj->enableUpdate(false);
			});
		
		updateFunctionsMap.emplace("default", [](IGameObject* obj) {});
		
		updateFunctionsMap.emplace("HorizontalDoor", [](IGameObject* obj)
			{
				auto TC = obj->getTransformComponent();
				auto MC = obj->getMovableComponent();
				auto PC = obj->getPhysicComponent();

				if (MC->speed == 0 || TC->rotation >= 130)
				{
					PC->walkHitBox->recalculateIShape(TC->position, TC->rotation);

					MC->speed = 0;
					return;
				}

				float sign = TC->rotation <= 150 ? 1.f : -1.f;
				TC->rotation += sign * MC->speed * time.getDeltaTime();
				std::cout << "update" << std::endl;

				//	PC->walkHitBox->recalculateIShape(TC->position, TC->rotation);
			});

		updateFunctionsMap.emplace("Bat", [](IGameObject* obj) 
			{
				auto MC = obj->getMovableComponent();

				if (MC->speed == 0)
					return;

				auto TC = obj->getTransformComponent();
				auto PC = obj->getPhysicComponent();

				TC->position.x += MC->velocity.x * MC->speed * time.getDeltaTime();
				TC->position.y += MC->velocity.y * MC->speed * time.getDeltaTime();
				TC->rotation += MC->speed * time.getDeltaTime() * 2;
				MC->speed -= 0.13 / time.getDeltaTime();

				if (MC->speed < 0) MC->speed = 0;

				PC->walkHitBox->recalculateIShape(TC->position, TC->rotation);

			});

		renderFunctionsMap.emplace("default", [](IGameObject* obj) 
			{
				auto RC = obj->getRenderComponent();
				auto TC = obj->getTransformComponent();
				auto SC = obj->getShadowComponent();

				if (SC)
				{
					auto shader = RC->sprite->getShader();
					RC->sprite->setShader(resources.getShader("shadowShader"));
					RC->sprite->render(TC->position + SC->objectOffset, TC->rotation, RC->size, RC->rotationPoint, RC->pivotOffset, SC->layer, RC->animator.getCurrentFrame());
					RC->sprite->setShader(shader);
				}

				RC->sprite->render(TC->position, TC->rotation, RC->size, RC->rotationPoint, RC->pivotOffset, RC->layer, RC->animator.getCurrentFrame());
			});

		renderFunctionsMap.emplace("Bat", [](IGameObject* obj)
			{
				auto RC = obj->getRenderComponent();
				auto TC = obj->getTransformComponent();
				auto SC = obj->getShadowComponent();

				if (SC)
				{
					auto shader = RC->sprite->getShader();
					RC->sprite->setShader(resources.getShader("shadowShader"));
					RC->sprite->render(TC->position + SC->objectOffset, TC->rotation, RC->size, RC->rotationPoint, RC->pivotOffset, SC->layer, RC->animator.getCurrentFrame());
					RC->sprite->setShader(shader);
				}

				RC->sprite->render(TC->position + glm::vec2(0, 1.5f * sin(time.getTime() * 0.005)), TC->rotation, RC->size, RC->rotationPoint, RC->pivotOffset, RC->layer, RC->animator.getCurrentFrame());
			});

		renderFunctionsMap.emplace("FilmGrain", [](IGameObject* obj)
			{
				auto RC = obj->getRenderComponent();
				auto TC = obj->getTransformComponent();

				RC->sprite->getShader()->setFloat("time", 4.f * sin(time.getTime()));
				std::cout << 20.f * sin(time.getTime()) << std::endl;
				RC->sprite->render(TC->position, TC->rotation, RC->size, RC->rotationPoint, RC->pivotOffset, RC->layer, RC->animator.getCurrentFrame());

			});
	}
	
	static std::function<void(IGameObject*, IGameObject*, const glm::vec2&)> getWalkHitBoxCallback(std::string& name)
	{
		if (walkHitBoxOnCollisionMap.count(name))
			return walkHitBoxOnCollisionMap[name];
		else
		{
			std::cout << "Cant find WalkHitBoxCallback! Callback name: " << name << std::endl;
			exit(-1);
		}
	}

	static std::function<void(IGameObject*)> getUpdateFunction(std::string& name)
	{
		if (updateFunctionsMap.count(name))
			return updateFunctionsMap[name];
		else
		{
			std::cout << "Cant find updateFunction! Function name: " << name << std::endl;
			exit(-1);
		}
	}

	static std::function<void(IGameObject*)> getRenderFunction(std::string& name)
	{
		if (renderFunctionsMap.count(name))
			return renderFunctionsMap[name];
		else
		{
			std::cout << "Cant find renderFunction! Function name: " << name << std::endl;
			exit(-1);
		}
	}

	MapObject(const std::string& tag,
		TransformComponent* const transformComponent,
		RenderComponent* const renderComponent,
		PhysicComponent* const physicComponent,
		MovableComponent* const movableComponent,
		ShadowComponent* const shadowComponent)
		: TC(transformComponent), RC(renderComponent), PC(physicComponent), MC(movableComponent), SC(shadowComponent), IGameObject(tag)
	{

		if (PC)
		{
			if (PC->damageHitBox)
				PC->damageHitBox->recalculateIShape(TC->position, TC->rotation);
			if (PC->walkHitBox)
				PC->walkHitBox->recalculateIShape(TC->position, TC->rotation);
		}
	}
	
	~MapObject()
	{
		if (!TC) delete TC;
		if (!RC) delete RC;
		if (!PC) delete PC;
		if (!MC) delete MC;
	}

	void render() override
	{
		if (needRender)
			renderFunction(this);
	}

	void update() override 
	{
		if (needUpdate)
			updateFunction(this);
	}

	PhysicComponent* const getPhysicComponent() override { return PC; }
	RenderComponent* const getRenderComponent() override { return RC; }
	TransformComponent* const getTransformComponent() override { return TC; }
	MovableComponent* const getMovableComponent() override { return MC; }
	ShadowComponent* const getShadowComponent() override { return SC; }

	void setUpdateFunction(std::function<void(IGameObject*)> updateFunction)
	{
		this->updateFunction = updateFunction;
	}

	void setRenderFunction(std::function<void(IGameObject*)> renderFunction)
	{
		this->renderFunction = renderFunction;
	}
};

std::map<std::string, std::function<void(IGameObject*, IGameObject*, const glm::vec2&)>> MapObject::walkHitBoxOnCollisionMap;
std::map<std::string, std::function<void(IGameObject*)>> MapObject::updateFunctionsMap;
std::map<std::string, std::function<void(IGameObject*)>> MapObject::renderFunctionsMap;
