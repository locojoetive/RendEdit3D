#include "Scene.h"
#include <regex>

Scene::Scene(Camera &_camera)
{
	camera = &_camera;
	defaultShader = new Shader("Resources/Shaders/default.vert", "Resources/Shaders/default.frag");
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

	for (Model m : models)
	{
		m.Draw(*defaultShader, *camera);
	}

	// updates and exports the camera matrix to the vertex shader
	// camera->updateMatrix();
	/*
	defaultShader->Activate();
	defaultShader->SetUniform1f("near", camera->getNearClipDistance());
	defaultShader->SetUniform1f("far", camera->getFarClipDistance());
	*/
}

void Scene::LoadModel(std::string path)
{
	// comes from ImGUI with "\\"
	// get_file_content(...) uses '/' 
	path = std::regex_replace(path, std::regex("\\\\"), "/");
	Model newModel(path.c_str());
	models.push_back(newModel);
}
