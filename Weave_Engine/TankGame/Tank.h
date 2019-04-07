#pragma once

#include <ECS/Component.h>

class Tank : public ECS::Component<Tank>
{
public:

	COMPONENT( Tank );

	Tank();
	~Tank();

	void Update(float deltaTime);

protected:

	glm::vec3 velocity;
};

