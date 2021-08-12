#pragma once

#include "Scene.h"
#include "Mesh.h"


class MainMenu : public Scene {

public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
};

