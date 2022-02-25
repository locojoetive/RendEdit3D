#pragma once

#include <vector>

#include "Camera.h"
#include "Model.h"
#include "Light.h"

class Scene
{
private:
	Camera* camera;
	Shader* defaultShader;
	std::vector<Model> models;
	std::vector<Light> lights;

public:
	Scene(Camera &_camera);
	~Scene();
	void UpdateScene();
	void LoadModel(std::string path);
};
