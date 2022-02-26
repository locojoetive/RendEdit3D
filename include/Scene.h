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
	std::vector <std::string> modelNames;

public:
	Light light;

	std::vector<Model>::iterator selectedModel;
	Scene(Camera &_camera);
	~Scene();
	void UpdateScene();
	void LoadModel(std::string path);
	bool isModelSelected();
	void selectModel(unsigned int index);
	std::vector<std::string> getModelNames();
};

