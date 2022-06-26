
/*
	void onPreRender() override
	{
		glm::vec2 focusPoint = objectInFocus->getComponent<TransformComponent>()->getGlobalPosition();
		auto curAnimation = objectInFocus->getComponent<SpriteRendererComponent>()->currentAnimation;
		focusPoint += curAnimation->localPosition + curAnimation->rotationPoint;

		glm::vec2 destinationPosition;

		destinationPosition.x = focusPoint.x - Window::size.x * 0.5f / scale.x;
		destinationPosition.y = focusPoint.y - Window::size.y * 0.5f / scale.y;
		destinationPosition.x = glm::clamp(destinationPosition.x, minPositionXY.x, maxPositionXY.x);
		destinationPosition.y = glm::clamp(destinationPosition.y, minPositionXY.y, maxPositionXY.y);

		float offset = InputHandler::getKeys()[GLFW_KEY_LEFT_SHIFT] == GLFW_PRESS ? maxOffset : minOffset;

		glm::vec2 deltaDistance = screenToWorldPoint(InputHandler::getMousePoint()) - (glm::vec2)focusPoint;
		glm::vec2 velocity = glm::normalize(deltaDistance);

		deltaDistance.x = abs(deltaDistance.x) > offset ? offset : abs(deltaDistance.x);
		deltaDistance.y = abs(deltaDistance.y) > offset ? offset : abs(deltaDistance.y);
		destinationPosition.x += velocity.x * deltaDistance.x;
		destinationPosition.y += velocity.y * deltaDistance.y;

		destinationPosition.x -= std::remainderf(destinationPosition.x, 1.f / 4.f);
		destinationPosition.y -= std::remainderf(destinationPosition.y, 1.f / 4.f);

		double deltaTime = Time::getDeltaTime();
		auto TC = gameObject->getComponent<TransformComponent>();
		deltaDistance = destinationPosition - TC->position;
		velocity = glm::normalize(deltaDistance);

		if (isnan(velocity.x)) velocity.x = 0;
		if (isnan(velocity.y)) velocity.y = 0;

		float speed = 0.4f;
		glm::vec2 curOffset = glm::vec2(velocity.x * speed * deltaTime, velocity.y * speed * deltaTime);

		if (abs(curOffset.x) > abs(deltaDistance.x))
			TC->position.x = destinationPosition.x;
		else
			TC->position.x += curOffset.x;

		if (abs(curOffset.y) > abs(deltaDistance.y))
			TC->position.y = destinationPosition.y;
		else
			TC->position.y += curOffset.y;


		recalculateMatrix();

		gameObject->getComponent<SpriteRendererComponent>()->shader->setFloat("time", 4.f * sin(Time::getTime()));

		ResourceManager::getShader("spriteShader")->setMatrix("viewProjectionMat", viewProjectionMatrix);
		ResourceManager::getShader("shadowShader")->setMatrix("viewProjectionMat", viewProjectionMatrix);
		ResourceManager::getShader("filmGrainShader")->setMatrix("viewProjectionMat", viewProjectionMatrix);
	}




	void onPreRender() override
	{
		glm::vec2 focusPoint = objectInFocus->getComponent<TransformComponent>()->getGlobalPosition();
		auto curAnimation = objectInFocus->getComponent<SpriteRendererComponent>()->currentAnimation;
		focusPoint += curAnimation->localPosition + curAnimation->rotationPoint;


		auto TC = gameObject->getComponent<TransformComponent>();
		TC->position.x = focusPoint.x - Window::size.x * 0.5f / scale.x;
		TC->position.y = focusPoint.y - Window::size.y * 0.5f / scale.y;

		float offset = InputHandler::getKeys()[GLFW_KEY_LEFT_SHIFT] == GLFW_PRESS ? maxOffset : minOffset;

		TC->position.x = glm::clamp(TC->position.x, minPositionXY.x, maxPositionXY.x);
		TC->position.y = glm::clamp(TC->position.y, minPositionXY.y, maxPositionXY.y);

		glm::vec2 deltaDistance = screenToWorldPoint(InputHandler::getMousePoint()) - (glm::vec2)focusPoint;
		glm::vec2 velocity = glm::normalize(deltaDistance);

		deltaDistance.x = abs(deltaDistance.x) > offset ? offset : abs(deltaDistance.x);
		deltaDistance.y = abs(deltaDistance.y) > offset ? offset : abs(deltaDistance.y);

		TC->position.x += velocity.x * deltaDistance.x;
		TC->position.y += velocity.y * deltaDistance.y;

		TC->position.x -= std::remainderf(TC->position.x, 1.f / 4.f);
		TC->position.y -= std::remainderf(TC->position.y, 1.f / 4.f);

		recalculateMatrix();

		gameObject->getComponent<SpriteRendererComponent>()->shader->setFloat("time", 4.f * sin(Time::getTime()));

		ResourceManager::getShader("spriteShader")->setMatrix("viewProjectionMat", viewProjectionMatrix);
		ResourceManager::getShader("shadowShader")->setMatrix("viewProjectionMat", viewProjectionMatrix);
		ResourceManager::getShader("filmGrainShader")->setMatrix("viewProjectionMat", viewProjectionMatrix);
	}
*/