#include "Scene.h"
#include <regex>

Scene::Scene(Camera &_camera)
{
	camera = &_camera;
	defaultShader = new Shader("Resources/Shaders/default.vert", "Resources/Shaders/default.frag");
	arrow = new Model("Resources/Models/_arrow/scene.gltf");
}


Scene::~Scene()
{
	defaultShader->Delete();
}

void Scene::UpdateScene()
{
	// camera->BindFrameBuffer();
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	arrow->Draw(*defaultShader, *camera);
	defaultShader->Activate();
	defaultShader->SetUniform4f("lightColor", light.getColor());
	defaultShader->SetUniform1f("near", camera->nearClipDistance);
	defaultShader->SetUniform1f("far", camera->farClipDistance);
	for (Model m : models)
	{
		m.Draw(*defaultShader, *camera);
	}

	// updates and exports the camera matrix to the vertex shader
	camera->updateMatrix();
}

void Scene::LoadModel(std::string path)
{
	// comes from ImGUI with "\\"
	// get_file_content(...) uses '/' 
	path = std::regex_replace(path, std::regex("\\\\"), "/");
	models.push_back(Model(path.c_str()));
	selectedModel = models.end()-1;
	modelNames.push_back(selectedModel->getName());
}

void Scene::DeleteSelectedModel()
{
	int index = selectedModel - models.begin();
	models.erase(selectedModel);
	modelNames.erase(modelNames.begin() + index);
	if (models.size() > 0)
	{
		selectedModel = models.end() - 1;
	}
	else
	{
		selectedModel = models.end();
	}
}

void Scene::selectModel(unsigned int index)
{
	selectedModel = models.begin() + index;
}

bool Scene::isModelSelected()
{
	return models.size() > 0 && selectedModel != models.end();
}

std::vector<std::string> Scene::getModelNames()
{
	return modelNames;
}